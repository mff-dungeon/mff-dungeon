#ifndef RECIPE_HPP
#define	RECIPE_HPP

#include "IDescriptable.hpp"

namespace Dungeon {
	/**
	 * A recipe for the crafter, controls the item creation
     */
	class Recipe : public IDescriptable {
	public:
		Recipe();
		Recipe(objId id);
		virtual ~Recipe();
		
		virtual string getDescription() const;
		
		int getLevelRequired() const;
		Recipe* setLevelRequired(int level);
		int getResource(int type);
		Recipe* setResource(int resource, int type);
		Recipe* setResourceList(int* resources);
		
		void tryCraft(ActionDescriptor* ad);
		
		virtual void getActions(ActionList* list, ObjectPointer callee) {};
		
		virtual void registerProperties(IPropertyStorage& storage);

	private:
		int level;
		int* resources;

	PERSISTENT_DECLARATION(Recipe, IObject)
	};
}

#endif	/* RECIPE_HPP */

