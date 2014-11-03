#ifndef TRAP_HPP
#define	TRAP_HPP

#include "../../common.hpp"

namespace Dungeon {

	/**
	 * Event handler, also known as Trap for DnD players.
	 * 
	 * Trap is a "virtual" object. It exists in the world, but it should not 
	 * be located anywhere, nor there should be any relation where trap is slave.
	 * If you make a relation trap --> object, and the event handlers are
	 * written properly, it should be triggered.
	 */
	class Trap : public Base {
	public:
		Trap() : Base() { };
		Trap(string id) : Base(id) { };
		virtual ~Trap() { };
		/**
		 * Can either process everything (and continue processing original action),
		 * or throw TrapException and stop it.
		 * 
		 * If the trap should not be triggered (probability, has skill etc.) 
		 * then just do nothing in this method.
		 * 
		 * @param event The event given to Base->triggerTraps
		 * @param target The target which triggered the trap
		 * @param ad Can be NULL sometimes, then use the exception trigger
		 */
		virtual void trigger(string event, ObjectPointer target, ActionDescriptor* ad) {
			LOG("Trap") << "Empty trap triggered." << LOGF;
		}

		/**
		 * This one will be called by driver after throwing TrapException.
		 * Trap can take care for the lifecycle of AD as following:
		 *  - if the action shall be repeated, do nothing & return true
		 *  - if no further action shall be processed, set action to null || return false
		 *  - to redirect, just set the action to the new one & return true
		 * 
		 * The return value was added because you may need to remember 
		 * action while interrupting it's processing.
		 * 
		 * TL;DR The action in ad, if any, will be run afterwards.
		 * 
		 * @return true if processing should continue, false otherwise
		 */
		virtual bool exceptionTrigger(ActionDescriptor* ad);
		/**
		 * This method should never be called on Trap.
		 */
		virtual void getActions(ActionList * list, ObjectPointer callee) {
			LOGS("Trap", Error) << "Getting actions od Trap? Srsly?" << LOGF;
		}
		/**
		 * Returns name of relation for that event
		 */
		static const string getRelation(const string event) {
			return "trap-" + event;
		}

		PERSISTENT_DECLARATION(Trap, Base)
	};

	/**
	 * Special exception that happens when a trap is triggered.
	 */
	class TrapException : public GameException {
	public:
		TrapException(const ObjectPointer& trap);

		ObjectPointer getTrap() const;

	private:
		ObjectPointer trap;
	};

	/**
	 * When there will be more of these, consider making special directory
	 */
	class DemoTrap : public Trap {
	public:
		DemoTrap() : Trap() { };
		DemoTrap(string id) : Trap(id) { };
		virtual ~DemoTrap() { };
		virtual void trigger(string event, ObjectPointer target, ActionDescriptor* ad);

		PERSISTENT_DECLARATION(DemoTrap, Trap)
	};
}

#endif

