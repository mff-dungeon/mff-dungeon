#ifndef RECIPE_HPP
#define	RECIPE_HPP

#include "IDescriptable.hpp"
#include "Human.hpp"

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
		
		int getLevel() const;
		Recipe* setLevel(int level);
		int getExperience() const;
		Recipe* setExperience(int experience);
		int getResource(int type) const;
		Recipe* setResource(int type, int resource);
		ObjectPointer getBadItem();
		Recipe* setBadItem(ObjectPointer item);
		ObjectPointer getGoodItem();
		Recipe* setGoodItem(ObjectPointer item);
		Human::Stats getMainStat() const;
		Recipe* setMainStat(Human::Stats stat);
		int getMainStatReq() const;
		
		Recipe* addStatReq(ObjectPointer reqPtr);
		bool checkStatReqs(ObjectPointer userPtr, ActionDescriptor* ad = 0);
		
		void tryCraft(ActionDescriptor* ad);
		
		virtual void getActions(ActionList* list, ObjectPointer callee) {};
		
		virtual void registerProperties(IPropertyStorage& storage);

	private:
		int level = 1;
		int experience = 0;
		Human::Stats mainStat = Human::Crafting;
		int* resources;

	PERSISTENT_DECLARATION(Recipe, IDescriptable)
	};
}

#endif	/* RECIPE_HPP */

