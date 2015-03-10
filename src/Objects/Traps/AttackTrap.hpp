#ifndef ATTACKTRAP_HPP
#define	ATTACKTRAP_HPP

#include "../../common.hpp"
#include "Trap.hpp"

namespace Dungeon {

	/**
	 * When in location, attack automatically
	 */
	class AttackTrap : public Trap {
	public:
		AttackTrap() : Trap() { }
		AttackTrap(const objId& id) : Trap(id) { };
		virtual ~AttackTrap() { };

		const static string doNotAttackRelation;

		virtual void trigger(const string& event, ObjectPointer room, ActionDescriptor* ad);
		virtual bool exceptionTrigger(ActionDescriptor* ad);

	private:
		ObjectPointer target;

		PERSISTENT_DECLARATION(AttackTrap, Trap)
	};
}

#endif

