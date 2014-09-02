#include "Creature.hpp"

namespace Dungeon {
	Creature::Creature() {

	}
	
	Creature::Creature(objId id) {
		this->setId(id);
	}

	Creature::~Creature() {

	}

	PERSISTENT_IMPLEMENTATION(Creature)
}