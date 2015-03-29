#include "ItemRespawner.hpp"
#include "../../Game/GameManager.hpp"

namespace Dungeon {

	ObjectPointer ItemRespawner::getTemplate() {
		return getSingleRelation("template");
	}

	ObjectPointer ItemRespawner::setTemplate(ObjectPointer templ) {
		setSingleRelation("template", templ);
		return this;
	}

	ObjectPointer ItemRespawner::doRespawn(ObjectPointer location) {
		ObjectPointer templ = getTemplate().assertExists("Template for respawn does not exist.");
		ObjectPointer inst = templ->deepClone();
		getGameManager()->clearRelationsOfType(inst, "template", Relation::Slave); // Can be template for more than one respawner
		inst->setSingleRelation(R_INSIDE, location, Relation::Slave);
		inst->attachTrap(this, "picked");
		setSingleRelation("last-created", inst);
		LOGS(Debug) << "Respawned " << templ.getId() << " as " << inst.getId() << LOGF;
		return inst;
	}

	void ItemRespawner::trigger(const string& event, ObjectPointer location, ActionDescriptor* ad) {
		if (event == "picked") {
			location->detachTrap(this, "picked");
			setTimestamp(Timestamp::Auto)->save();
			return;
		}

		if (mode == AfterDisappear) {
			ObjectPointer lastCreated = getSingleRelation("last-created");

			// If the item is still there, push the time
			if (!!lastCreated && lastCreated->hasRelation(R_INSIDE, location, Relation::Slave)) {
				setTimestamp(Timestamp::Auto)->save();
				return;
			}
		}

		if (getTimestamp() == Timestamp::NotNow) return;

		if (getTimestamp() <= std::time(nullptr)) {
			doRespawn(location);
			setTimestamp(Timestamp::Auto);
			if (mode == AfterPick)
				setTimestamp(Timestamp::NotNow);
			save();
			return;
		}
	}

	void ItemRespawner::registerProperties(IPropertyStorage& storage) {
		storage.have(interval, "respawner-interval", "Respawning interval")
				.have(timestamp, "respawner-timestamp", "Time at which the item respawns", false);
		Trap::registerProperties(storage);
	}


	PERSISTENT_IMPLEMENTATION(ItemRespawner)
}

