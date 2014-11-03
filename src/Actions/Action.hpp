#ifndef ACTION_HPP
#define	ACTION_HPP

#include "../common.hpp"

namespace Dungeon {

	class ActionList;

	/*
	 * The way objects interact with each other.
	 */
	class Action {
	public:
		Action(string type, bool visible = true) : type(type), isVisibleInHelp(visible) { }
		virtual ~Action() { }

		/**
		 * Returns true when this action should be commited for this message.
		 * Shall also fetch all needed parameters from the string and store it
		 * inside.
		 */
		virtual bool match(string command, ActionDescriptor* ad);
		/**
		 * Can be overriden to check integrity before commiting.
		 */
		virtual void validate() { }

		/**
		 * Commits the action.
		 */
		virtual void commit(ActionDescriptor *ad);

		/**
		 * Used in help text. Should fill the reply 
		 * in ad with some useful information.
		 * 
		 * If this action should not be visible, then just do nothing.
		 */
		virtual void explain(ActionDescriptor *ad);

		/**
		 * Handles arosen in-game exception. Can be called only after matching,
		 * or in dialog (where most errors can occur).
		 * @param exception
		 * @return true, if the error was handled properly and user
		 *         should not be notified any further
		 */
		virtual bool handleException(GameException& exception, ActionDescriptor *ad);

		string type;

		bool isVisibleInHelp;
		/**
		 * Basically implementation of shared pointers, 
		 * but with more comfortable syntax
		 */
		Action* remember() {
			ptrCount++;
			return this;
		}
		void forget() {
			if (--ptrCount <= 0)
				delete this;
		}

	private:
		int ptrCount = 0;
	};
}

#endif	/* ACTION_HPP */

