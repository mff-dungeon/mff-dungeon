#ifndef MULTITARGETACTION_HPP
#define	MULTITARGETACTION_HPP

#include "../common.hpp"
#include "../Objects/Traps/MTATrap.hpp"
#include "Action.hpp"

namespace Dungeon {

	/**
	 * Action that applies to more elements, merges
	 * with another actions of the same type
	 * and helps select matching target
	 */
	class MultiTargetAction : public Action {
    friend class MTATrap;
	public:
                
		MultiTargetAction(const string& type) : Action(type) { }

		/**
		 * Invoked by ActionList, when other action of the same type arrives.
		 * Should merge the lists of targets and delete the second action.
		 * @param second The other action of the same type
		 */
		virtual void merge(MultiTargetAction* second);

		/**
		 * Adds another potential target for this action.
		 * @param op Target
		 */
		virtual MultiTargetAction* addTarget(ObjectPointer op);

		const ObjectMap& getTargets() const;

		/**
		 * Like commit in general action, but this time with selected target.
		 * @param ad
		 * @param target Selected target
		 */
		virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target) = 0;
                
		virtual void selectBestTarget(const string& str, ActionDescriptor* ad);
                
		ObjectPointer getTarget() const {
			return selectedTarget;
		}
		void setTarget(const ObjectPointer& selectedTarget) {
			this->selectedTarget = selectedTarget;
		}
		virtual void validate() {
			if (!this->selectedTarget)
				throw GameException("Target must be set prior to committing.");
		}

		virtual void commit(ActionDescriptor* ad);

		void useActionFor(ObjectPointer target, ActionDescriptor* ad);

	protected:
		ObjectMap targets;
		ObjectPointer selectedTarget;
                
		/**
		 * Messages used by MTATrap in dialog. Overwrite in ctor to suit your needs.
		 */
		MTATrap::Messages messages;
        MTATrap::Info mtatrap_info;
	};
}

#endif	/* MULTITARGETACTION_HPP */

