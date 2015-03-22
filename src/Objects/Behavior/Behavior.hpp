#ifndef BEHAVIOR_HPP
#define	BEHAVIOR_HPP

#include "../../common.hpp"
#include "../../Actions/MultiTargetAction.hpp"

namespace Dungeon {

	/**
	 * Virtual object that implements modular behavior of other objects.
	 * Simple examle is the basic interaction, which makes any object
	 * able to be read.
	 */
	class Behavior : public Base {
	public:

		Behavior() { }
		Behavior(const objId& id) : Base(id) { }

		const string& getVerb() const
		{
			return verb;
		}

		Behavior* setVerb(const string& verb)
		{
			this->verb = verb;
			return this;
		}

		const string& getExplanation() const
		{
			return explanation;
		}

		Behavior* setExplanation(const string& explanation)
		{
			this->explanation = explanation;
			return this;
		}

		virtual void registerProperties(IPropertyStorage& storage);

		/**
		 * These will be merged together in some near future. Temporary HACK.
		 */
		virtual void getActions(ActionList* list, ObjectPointer callee);
		virtual void getActions(ActionList* list, ObjectPointer callee, ObjectPointer target);


		/**
		 * The main method that triggers the behavior.
		 */
		virtual void trigger(ObjectPointer target, ActionDescriptor* ad) = 0;

	private:
		string verb = "", explanation = "";

		NONPERSISTENT_DECLARATION(Behavior, Base)
	};

	class BehaviorAction : public MultiTargetAction {
	public:
		BehaviorAction(const string& type, ObjectPointer behavior);

		virtual void explain(ActionDescriptor* ad);
		virtual bool match(const string& command, ActionDescriptor* ad);
		virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);

	private:
		ObjectPointer behavior;
	};
}

#endif

