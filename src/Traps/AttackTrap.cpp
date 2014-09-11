#include "AttackTrap.hpp"
#include "../Objects/Creature.hpp"
#include "../RandomString.hpp"
#include "../ActionDescriptor.hpp"

namespace Dungeon {
	
	PERSISTENT_IMPLEMENTATION(AttackTrap)
	
	void AttackTrap::trigger(string event, ObjectPointer room, ActionDescriptor* ad) {
		if (!ad || ad->state != ActionDescriptor::RoundEnd) {
			LOGS("AttackTrap", Verbose) << "No AD or wrong time"<< LOGF;
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
			LOGS("AttackTrap", Verbose) << "No creature" << LOGF;
			return;
		}
		
		target.assertType<Creature>("Attack trap set for non-creature");
		if (target.unsafeCast<Creature>()->getState() != Alive::Living) {
			LOGS("AttackTrap", Verbose) << "Creature dead" << LOGF;
			return;
		}
		
		throw TrapException(this);
	}
	
	void AttackTrap::exceptionTrigger(ActionDescriptor* ad) {
		string name = target.unsafeCast<Creature>()->getName();
		*ad << (RandomString::get()
			<<  name + " has immedietely attacked you. " << endr
			<< "You've seen some motion, that was  " + name +  " attacking you. " << endr
			<< "Before you could have done anything else, " + name + " striked. " << endr);
		
		CombatAction* c = new CombatAction;
		c->addTarget(target);
		ad->setAction(c);
		target = nullptr;
	}
}

