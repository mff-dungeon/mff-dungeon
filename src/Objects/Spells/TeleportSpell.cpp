#include "TeleportSpell.hpp"
#include "../../Game/GameManager.hpp"
#include "../Human.hpp"
#include "../Location.hpp"

namespace Dungeon {

	TeleportSpell::TeleportSpell() { }

	TeleportSpell::TeleportSpell(objId id) : Spell(id) { }

	TeleportSpell::~TeleportSpell() { }

	ObjectPointer TeleportSpell::getDestination() {
		return getSingleRelation("telespell-dest", Relation::Master);
	}

	TeleportSpell* TeleportSpell::setDestination(ObjectPointer locationPtr) {
		locationPtr.assertExists("Where do you want to teleport?")
				.assertType<Location>("Only locations can be your destinations");
		setSingleRelation("telespell-dest", locationPtr, Relation::Master);
		return this;
	}

	void TeleportSpell::cast(ObjectPointer casterPtr, ActionDescriptor* ad) {
		Human* caster = casterPtr.assertExists("The caster doesn't exist")
				.assertType<Human>("Only humans cast spells")
				.unsafeCast<Human>();
		caster->changeResourceQuantity(Resource::ManaShard, -(getManaCost(casterPtr)));
		*ad << "You have cast " << this->getName() << "." << eos;
		ObjectPointer dest = getDestination();
		if (dest != nullptr) {
			*ad << "You have teleported to " << dest.unsafeCast<Location>()->getName() << "." << eos;
			getGameManager()->moveAlive(caster, dest);
		} else {
			dest = caster->getRespawnLocation();
			*ad << "You have teleported to " << dest.unsafeCast<Location>()->getName() << "." << eos;
			getGameManager()->moveAlive(caster, dest);
		}
	}

	PERSISTENT_IMPLEMENTATION(TeleportSpell)
}