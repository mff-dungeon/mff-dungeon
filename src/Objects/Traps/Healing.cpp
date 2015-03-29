#include "Healing.hpp"
#include "../Location.hpp"
#include "../Alive.hpp"

namespace Dungeon {

	Healing* Healing::setTarget(ObjectPointer target) {
		if (target->instanceOf(Location)) {
			target->attachTrap(this, "inside");
		} else if (target->instanceOf(Alive)) {
			target->attachTrap(this, "get-actions");
		} else throw new InvalidType("Healed target must be either Alive or Location.");

		if (!!getTarget()) {
			getTarget()->detachTrap(this, "inside");
			getTarget()->detachTrap(this, "get-actions");
		}
		setSingleRelation(R_TARGET, target);
		return this;
	}

	void Healing::trigger(const string& event, ObjectPointer trigger, ActionDescriptor* ad) {
		if (timestamp == 0) {
			timestamp = std::time(nullptr);
		}

		int difference = std::time(nullptr) - timestamp;
		int healedHp = difference * rate / PerSec;
		if (healedHp > 0) {
			difference = healedHp * PerSec / rate; // "floor" the last seconds which gave partial hp
			timestamp += difference;

			ObjectPointer target = getTarget();
			if (target->instanceOf(Alive)) {
				target.unsafeCast<Alive>()->changeHp(healedHp, ad);
				LOGS(Debug) << "Healed " << target->getId() << " for " << healedHp << LOGF;
			} else if (target->instanceOf(Location)) {
				for (auto obj : target->getRelations(Relation::Master, R_INSIDE)) {
					if (obj.second->instanceOf(Alive))
						obj.second.unsafeCast<Alive>()->changeHp(healedHp, ad);
				}
				LOGS(Debug) << "Healed everything in " << target.getId() << " for " << healedHp << LOGF;
			}

			save();
		}
	}

	void Healing::registerProperties(IPropertyStorage& storage) {
		storage.have(rate, "healing-rate", "Heal rate - hp per hour")
				.have(timestamp, "healing-timestamp", "Last full life given", false);
		Trap::registerProperties(storage);
	}

	PERSISTENT_IMPLEMENTATION(Healing)
}

