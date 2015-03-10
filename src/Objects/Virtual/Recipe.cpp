#include "Recipe.hpp"
#include "Stats.hpp"
#include "../../Utils/SentenceJoiner.hpp"
#include "../../Game/GameManager.hpp"
#include "../Resource.hpp"
#include "../Alive.hpp"
#include "../Human.hpp"
#include "../Inventory.hpp"

namespace Dungeon {

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
		for (int i = Resource::ManaShard; i >= 0; i--) {
			if (getResource(i) > 0) {
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

	Human::Stats Recipe::getMainStat() const {
		return mainStat;
	}

	Recipe* Recipe::setMainStat(Human::Stats stat) {
		this->mainStat = stat;
		return this;
	}

	int Recipe::getMainStatReq() const {
		int retVal = 0;
		try {
			const ObjectMap& reqs = getRelations(Relation::Master, R_REQUIREMENT);
			for (auto& req : reqs) {
				req.second.assertType<StatReq>("Only a requirement is supposed to be there.");
				StatReq* r = req.second.unsafeCast<StatReq>();
				if (r->getStat() == getMainStat()) {
					if (retVal < r->getValue()) retVal = r->getValue();
				}
			}
		} catch (std::out_of_range& e) {

		}
		return retVal;
	}

	Recipe* Recipe::addStatReq(ObjectPointer reqPtr) {
		reqPtr.assertExists("The requirement doesn't exist.")
				.assertType<StatReq>("You can only add a stat requirement");
		StatReq* req = reqPtr.unsafeCast<StatReq>();
		getGameManager()->createRelation(this, req, R_REQUIREMENT);
		return this;
	}

	bool Recipe::checkStatReqs(ObjectPointer userPtr, ActionDescriptor* ad) {
		userPtr.assertExists("The human is weird.")
				.assertType<Human>("Only a human has stats.");
		Human* user = userPtr.unsafeCast<Human>();
		try {
			const ObjectMap& reqs = getRelations(Relation::Master, R_REQUIREMENT);
			for (auto& req : reqs) {
				req.second.assertType<StatReq>("Only a requirement is supposed to be there.");
				StatReq* r = req.second.unsafeCast<StatReq>();
				if (user->getStatValue(r->getStat()) < r->getValue()) {
					*ad << "Your " << user->getStatName(r->getStat()) << " is not high enough." << eos;
					return false;
				}
			}
		} catch (std::out_of_range& e) {

		}
		return true;
	}

	void Recipe::tryCraft(ActionDescriptor* ad) {
		LOGS("Recipe", Debug) << "Requested crafting item" << LOGF;
		if (!checkStatReqs(ad->getCaller(), ad)) return; // Stats not high enough
		for (int i = Resource::ManaShard; i >= 0; i--) {
			if (!ad->getCaller()->hasResourceGreaterThan((Resource::ResourceType) i, getResource(i))) {
				*ad << "You don't have enough " << Resource::ResourceName[i] << " to craft " << getName() << "." << eos;
				return;
			}
		}
		for (int i = Resource::ManaShard; i >= 0; i--) {
			ad->getCaller()->changeResourceQuantity((Resource::ResourceType) i, -getResource(i));
		}
		Human* crafter = ad->getCaller();
		int levelDiff = crafter->getStatValue(getMainStat()) - getMainStatReq();

		double failRate = 5000 * (2 - 2.0 / (levelDiff + 2));
		if (failRate < Utils::getRandomInt(1, 10000)) {
			*ad << "You have failed creating " << getName() << ". Maybe next time." << eos;
			crafter->addExperience(getExperience() / 10);
			return;
		}

		Item* created = 0;
		double successRate = 10000 * (1 - 10.0 / (levelDiff + 11));
		if (!getBadItem()) successRate = 10000; // Always do the good item
		if (successRate < Utils::getRandomInt(1, 10000)) {
			*ad << "You have tried to craft " << getName() << " but you succeeded just barely." << eos;
			created = getBadItem().assertExists("The crafting template doesn't exist")
					.assertType<Item>("What is that? Crafting a non-item?")
					->shallowClone().safeCast<Item>();
			crafter->addExperience(getExperience() / 2);
		} else {
			*ad << "You have managed to craft " << getName() << "!" << eos;
			created = getGoodItem().assertExists("The crafting template doesn't exist")
					.assertType<Item>("What is that? Crafting a non-item?")
					->shallowClone().safeCast<Item>();
			crafter->addExperience(getExperience());
		}

		Inventory* backpack = crafter->getBackpack().safeCast<Inventory>();
		if (backpack->canAdd(created)) {
			backpack->addItem(created);
			*ad << "You have acquired a " << created->getName() << " and put it in your backpack." << eos;
		} else {
			*ad << "You have created a " << created->getName() << " but as you don't have free space in your backpack "
					<< "you have dropped it on the ground." << eos;
			created->setSingleRelation(R_INSIDE, crafter->getLocation(), Relation::Slave);
		}
	}

	void Recipe::registerProperties(IPropertyStorage& storage) {
		storage.have(level, "recipe-level", "Level required to craft the item.")
				.have(experience, "recipe-exp", "Experience gained for crafting this item.")
				.have((int&) mainStat, "recipe-mainstat", "Main stat required for this recipe (affects success).");
		for (int i = Resource::ManaShard; i >= 0; i--) {
			storage.have(resources[i], string("recipe-resource-") + Resource::ResourceName[i],
					string("Resource ") + Resource::ResourceName[i] + " required.");
		}
		IDescriptable::registerProperties(storage);
	}

	PERSISTENT_IMPLEMENTATION(Recipe)
}


