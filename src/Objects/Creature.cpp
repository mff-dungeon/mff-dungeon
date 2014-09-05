#include "Creature.hpp"
#include <time.h>

namespace Dungeon {

	Alive* Creature::die(ActionDescriptor* ad) {
		if(this->getRespawnInterval() == -1) { // Remove
			// FIXME: Workout a way to tell GM, to delete this after this action
			this->setState(State::Invalid);
			return 0;
		}
		else {
			this->setRespawnTime(time(0) + getRespawnInterval());
			this->setState(State::Dead);
		}
		if(ad != 0) {
			*ad << "You have killed " << getName() << ". ";
		}
		
		return this;
	}

	Alive* Creature::respawn(ActionDescriptor* ad) {
		this->setCurrentHp(getMaxHp());
		this->setState(State::Living);
		return this;
	}

	void Creature::getActions(ActionList* list, ObjectPointer callee) {
		Alive::getActions(list, callee);
		// Something like examine should be here - can also be used on dead body (with different explaining)
		// Halfly Implemented to not forget
		if(getState() == State::Dead) {
			if(time(0) >= getRespawnTime()) {
				respawn();
				save();
			}
			else {
				return;
			}
		}
		// Rest of the actions
	}

	
	
	PERSISTENT_IMPLEMENTATION(Creature)
}