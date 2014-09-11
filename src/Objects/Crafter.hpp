#ifndef CRAFTER_HPP
#define	CRAFTER_HPP

#include "IDescriptable.hpp"
#include "Recipe.hpp"
#include "Alive.hpp"
#include "Human.hpp"
#include "../Actions/MultiTargetAction.hpp"

namespace Dungeon {
	/**
	 * An object in the world allowing user to craft some items from its recipes.
	 * Could be a magical table, a smithing anvil, ...
     */
	class Crafter : public IDescriptable {
	public:
		Crafter();
		Crafter(objId id);
		virtual ~Crafter();
		
		Crafter* addRecipe(ObjectPointer recipe);
		Crafter* removeRecipe(ObjectPointer recipe);

		virtual void getActions(ActionList* list, ObjectPointer callee);

	private:
		
	PERSISTENT_DECLARATION(Crafter, IDescriptable)
	};
	
	class CraftAction : public MultiTargetAction {
	public:
		CraftAction(string type = "crafter-craft") : MultiTargetAction(type) {}
        
        virtual void explain(ActionDescriptor* ad);
        virtual bool matchCommand(string command);
        virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);

        virtual void commit(ActionDescriptor* ad);
	};
	
	class CreateAction : public MultiTargetAction {
	public:
		CreateAction(string type = "crafter-create") : MultiTargetAction(type) {};
        
        virtual void explain(ActionDescriptor* ad);
        virtual bool matchCommand(string command);
        virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);

        virtual void commit(ActionDescriptor* ad);
	};
}

#endif	/* CRAFTER_HPP */

