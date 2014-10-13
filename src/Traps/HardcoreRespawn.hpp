#ifndef HARDCORERESPAWN_HPP
#define	HARDCORERESPAWN_HPP

#include "Trap.hpp"

namespace Dungeon {

	/**
	 *	Trap taking care of recreating people on death in hardcore mode
	 *	TODO: decide, what to do with dead characters and all their items/stuff
	 */
	class HardcoreRespawn : public Trap {
	public:
		HardcoreRespawn() { };
		HardcoreRespawn(objId id) : Trap(id) { };
		virtual ~HardcoreRespawn() { };

		virtual void trigger(string event, ObjectPointer target, ActionDescriptor* ad);
		virtual bool exceptionTrigger(ActionDescriptor* ad);

	private:

		PERSISTENT_DECLARATION(HardcoreRespawn, Trap)
	};
}

#endif	/* HARDCORERESPAWN_HPP */

