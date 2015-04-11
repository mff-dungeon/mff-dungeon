#ifndef CRAFTER_HPP
#define	CRAFTER_HPP

#include "../common.hpp"
#include "../Actions/MultiTargetAction.hpp"
#include "Virtual/IDescriptable.hpp"

namespace Dungeon {

	/**
	 * An object in the world allowing user to craft some items from its recipes.
	 * Could be a magical table, a smithing anvil, ...
	 */
	class Crafter : public IDescriptable {
	public:
		Crafter() {}
		Crafter(const objId& id) : IDescriptable(id) {};
		virtual ~Crafter() {}

		Crafter* addRecipe(ObjectPointer recipe);
		Crafter* removeRecipe(ObjectPointer recipe);

		virtual void getActions(ActionDescriptor* ad);

	private:
		PERSISTENT_DECLARATION(Crafter, IDescriptable)
	};

	class CraftAction : public MultiTargetAction {
	public:
		CraftAction(const string& type = "crafter-craft") : MultiTargetAction(type) { }

		virtual void explain(ActionDescriptor* ad);
		virtual bool match(const string& command, ActionDescriptor* ad);
		virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);
	};

	class CreateAction : public MultiTargetAction {
	public:
		CreateAction(const string& type = "crafter-create") : MultiTargetAction(type) { };

		virtual void explain(ActionDescriptor* ad);
		virtual bool match(const string& command, ActionDescriptor* ad);
		virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);
	};
}

#endif	/* CRAFTER_HPP */

