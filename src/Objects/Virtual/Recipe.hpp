#ifndef RECIPE_HPP
#define	RECIPE_HPP

#include "../Human.hpp"
#include "IDescriptable.hpp"

namespace Dungeon {

	/**
	 * A recipe for the crafter, controls the item creation
	 */
	class Recipe : public IDescriptable {
	public:
		Recipe() { initResources(); }
		Recipe(const objId& id) : IDescriptable(id) { initResources(); }
		virtual ~Recipe() { }

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

                bool hasEnoughMaterial(ObjectPointer human);
                
		void tryCraft(ActionDescriptor* ad);
		virtual void getActions(ActionDescriptor* ad) { };

		virtual void registerProperties(IPropertyStorage& storage);

	private:
		int level = 1;
		int experience = 0;
		Human::Stats mainStat = Human::Crafting;
		void initResources();

		int resources [Resource::Count];

		PERSISTENT_DECLARATION(Recipe, IDescriptable)
	};
}

#endif	/* RECIPE_HPP */

