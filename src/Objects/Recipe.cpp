#include "Recipe.hpp"
#include "Resource.hpp"
#include "Alive.hpp"
#include "Human.hpp"
#include "Inventory.hpp"
#include "../SentenceJoiner.hpp"

namespace Dungeon {

	Recipe::Recipe() {
		this->resources = new int[Resource::ManaShard+1];
		for(int i = 0; i<=Resource::ManaShard; i++) resources[i] = 0;
	}
	
	Recipe::Recipe(objId id) : IDescriptable(id) {
		this->resources = new int[Resource::ManaShard+1];
		for(int i = 0; i<=Resource::ManaShard; i++) resources[i] = 0;
	}

	Recipe::~Recipe() {
		delete[] this->resources;
	}

	int Recipe::getLevel() const {
		return level;
	}

	Recipe* Recipe::setLevel(int level) {
		this->level = level;
		return this;
	}

	int Recipe::getExperience() const {
		return experience;
	}

	Recipe* Recipe::setExperience(int experience) {
		this->experience = experience;
		return this;
	}

	ObjectPointer Recipe::getBadItem() {
		return getSingleRelation("recipeitem-bad", Relation::Master);
	}

	Recipe* Recipe::setBadItem(ObjectPointer item) {
		setSingleRelation("recipeitem-bad", item, Relation::Master);
		return this;
	}

	ObjectPointer Recipe::getGoodItem() {
		return getSingleRelation("recipeitem-good", Relation::Master);
	}
	
	Recipe* Recipe::setGoodItem(ObjectPointer item) {
		setSingleRelation("recipeitem-good", item, Relation::Master);
		return this;
	}

	string Recipe::getDescription() const {
		SentenceJoiner joiner;
		for(int i = Resource::ManaShard; i>=0; i--) {
			if(getResource(i) > 0) {
				joiner << to_string(getResource(i)) + " " + Resource::ResourceName[i];
			}
		}
		return joiner.getSentence("", getName() + ": %.");
	}

	int Recipe::getResource(int type) const {
		return resources[type];
	}

	Recipe* Recipe::setResource(int type, int resource) {
		this->resources[type] = resource;
		return this;
	}

	void Recipe::tryCraft(ActionDescriptor* ad) {
		for(int i = Resource::ManaShard; i>=0; i--) {
			if(!ad->getAlive()->hasResourceGreaterThan((Resource::ResourceType) i, getResource(i))) {
				*ad << "You don't have enough " << Resource::ResourceName[i] << " to craft " << getName() << ". ";
				return;
			}
		}
		for(int i = Resource::ManaShard; i>=0; i--) {
			ad->getAlive()->changeResourceQuantity((Resource::ResourceType) i, -getResource(i));
		}
		Human* crafter = (Human*) ad->getAlive();
		int levelDiff = crafter->getCraftingLevel() - getLevel();
		
		double failRate = 5000*(2-2.0/(levelDiff+2));
		if(failRate < Utils::getRandomInt(1, 10000)) {
			*ad << "You have failed creating " << getName() << ". Maybe next time";
			crafter->addCraftingExp(getExperience()/10);
			return;
		}
		
		Item* created = 0;
		double successRate = 10000*(1-10.0/(levelDiff+11));
		if(getBadItem() == nullptr) successRate = 10000; // Always do the good item
		if(successRate < Utils::getRandomInt(1, 10000)) {
			*ad << "You have tried to craft " << getName() << " but you succeeded just barely. ";
			created = getBadItem().assertExists("The crafting template doesn't exist")
							.assertType<Item>("What is that? Crafting a non-item?")
							->shallowClone().safeCast<Item>();
			crafter->addCraftingExp(getExperience()/2);
		}
		else {
			*ad << "You have managed to craft " << getName() << "! ";
			created = getGoodItem().assertExists("The crafting template doesn't exist")
							.assertType<Item>("What is that? Crafting a non-item?")
							->shallowClone().safeCast<Item>();
			crafter->addCraftingExp(getExperience());
		}
		
		Inventory* backpack = crafter->getBackpack().safeCast<Inventory>();
		if(backpack->canAdd(created)) {
			backpack->addItem(created);
			*ad << "You have acquired a " << created->getName() << " and put it in your backpack. ";
		}
		else {
			*ad << "You have created a " << created->getName() << " but as you don't have free space in your backpack " 
					<< "you have dropped it on the ground. ";
			created->setSingleRelation(R_INSIDE, crafter->getLocation(), Relation::Slave);
		}
	}

	void Recipe::registerProperties(IPropertyStorage& storage) {
		storage.have(level, "recipe-level", "Level required to craft the item. ")
				.have(experience, "recipe-exp", "Experience gained for crafting this item. ");
		for(int i=Resource::ManaShard; i>=0; i--) {
			storage.have(resources[i], string("recipe-resource-") + Resource::ResourceName[i],
					string("Resource ") + Resource::ResourceName[i] + " required. ");
		}
		IDescriptable::registerProperties(storage);
	}

	PERSISTENT_IMPLEMENTATION(Recipe)
}


