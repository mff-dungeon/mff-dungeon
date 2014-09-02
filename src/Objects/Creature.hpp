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
		Creature();
		Creature(objId id);
		virtual ~Creature();
	private:

	PERSISTENT_DECLARATION(Creature, Alive)
	};
}

#endif	/* CREATURE_HPP */

