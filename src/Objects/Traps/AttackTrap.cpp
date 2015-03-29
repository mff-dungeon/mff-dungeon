#include "AttackTrap.hpp"
#include "../../Utils/RandomString.hpp"
#include "../../Game/ActionDescriptor.hpp"
#include "../Creature.hpp"

namespace Dungeon {

	PERSISTENT_IMPLEMENTATION(AttackTrap)

	void AttackTrap::trigger(const string& event, ObjectPointer room, ActionDescriptor* ad) {
		if (!ad || ad->state != ActionDescriptor::RoundEnd || ad->getCaller()->getState() == Alive::Dead) {
			LOGS(Debug) << "No AD or wrong time" << LOGF;
			return;
		}
		target = getSingleRelation(R_TARGET);
		if (!target) {
			// Find some creature in that room (not necessarily room, but...)
			for (const ObjectMap::value_type& pair : room->getRelations(Relation::Master, R_INSIDE))
				if (pair.second->instanceOf(Creature) && pair.second.unsafeCast<Creature>()->getState() == Alive::Living)
					target = pair.second;
		}
		if (!target) {
			LOGS(Debug) << "No creature" << LOGF;
			return;
		}

		target.assertType<Creature>("Attack trap set for non-creature");
		if (target.unsafeCast<Creature>()->getState() != Alive::Living) {
			LOGS(Debug) << "Creature dead" << LOGF;
			return;
		}

		throw TrapException(this);
	}

	bool AttackTrap::exceptionTrigger(ActionDescriptor* ad) {
		string name = target.unsafeCast<Creature>()->getName();
		*ad << (RandomString::get()
				<< name + " has immedietely attacked you." << endr
				<< "You've seen some motion, that was  " + name + " attacking you." << endr
				<< "Before you could have done anything else, " + name + " striked." << endr) << eos;

		CombatAction* c = new CombatAction;
		c->setTarget(target);
		ad->setAction(c);
		target = nullptr;
		return true;
	}
}

