#ifndef CREATURE_HPP
#define	CREATURE_HPP

#include "../common.hpp"
#include "Alive.hpp"

namespace Dungeon {
	
	/**
	 * Represents every attackable enemy creature
	 * Registers attack/finish actions
     */
	class Creature : public Alive {
	public:
		Creature() {}
		Creature(objId id) : Alive(id) {};
		virtual ~Creature() {}
	
		virtual Alive* die(ActionDescriptor* ad = 0);
		virtual Alive* respawn(ActionDescriptor* ad = 0);

		virtual void getActions(ActionList* list, ObjectPointer callee);

	private:

	PERSISTENT_DECLARATION(Creature, Alive)
	};
}

#endif	/* CREATURE_HPP */

