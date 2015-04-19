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
	public:
                /**
                 * Messages used by MTATrap in dialog. Copy and declare to suit your needs.
                 */
                struct DefaultMessages {
                    static const char *unspecifiedMore, *unspecifiedOne,
                        *chosenForYou, *dontCare, *uncertain, *nextTime,
                        *totallyUncertain, *noCandidate, *commonRegex;
                };
                
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

		/**
		 * Does generic object matching to set best target
		 */
                template <typename Messages = DefaultMessages>
		void selectBestTarget(const string& str, ActionDescriptor* ad);
                
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
	};
}

#endif	/* MULTITARGETACTION_HPP */

