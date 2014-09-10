#include "Trap.hpp"
#include "../ActionDescriptor.hpp"
#include "Creature.hpp"
#include "../RandomString.hpp"

namespace Dungeon {
	
	PERSISTENT_IMPLEMENTATION(Trap)
	
	TrapException::TrapException(const ObjectPointer& trap)
	: GameException("Trap was triggered and driver handled it wrong.") {
		trap.assertExists("Triggered trap does not exist")
				.assertType<Trap>("Something that should be trap is not trap.");

		this->trap = trap;
		this->trap.setLock();
	}
	
	void Trap::exceptionTrigger(ActionDescriptor* ad) {
		ad->setAction(nullptr);
	}
	
	ObjectPointer TrapException::getTrap() const {
		return trap;
	}
	

	PERSISTENT_IMPLEMENTATION(DemoTrap)
	
	void DemoTrap::trigger(string event, ObjectPointer target, ActionDescriptor* ad) {
		LOG("DemoTrap") << "Trap on " << target.getId() << ", event " << event << " triggered" << LOGF;
		if (ad) 
			*ad << " [" + target.getId() + " // " + event + "] ";
	}	
	
	PERSISTENT_IMPLEMENTATION(AttackTrap)
	
	void AttackTrap::trigger(string event, ObjectPointer room, ActionDescriptor* ad) {
		if (!ad || ad->state != ActionDescriptor::RoundEnd) {
			LOGS("AttackTrap", Verbose) << "No AD or wrong time"<< LOGF;
			return;
		}
		target = getSingleRelation(R_TARGET);
		if (!target) {
			// Find some creature in that room (not necessarily room, but...)
			for (ObjectMap::value_type& pair : room->getRelations(Relation::Master, R_INSIDE))
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

