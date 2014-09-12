#include <cstring>

#include "Item.hpp"
#include "../Actions/CallbackAction.hpp"
#include "../ActionDescriptor.hpp"
#include "../RandomString.hpp"
#include "../SentenceJoiner.hpp"
#include "../Traps/ItemRespawner.hpp"

namespace Dungeon {
	
	Item::Item() {
		this->setDropable(true)->setPickable(true)->setSize(0)->setWeight(0);
	}
	
	Item::Item(objId id) : IDescriptable(id) {
		this->setDropable(true)->setPickable(true)->setSize(0)->setWeight(0);
	}

	Item::~Item() {

	}
	
	Item* Item::setSize(int size) {
		this->size = size;
		return this;
	}
	
	int Item::getSize() const {
		return this->size;
	}
	
	Item* Item::setWeight(int weight) {
		this->weight = weight;
		return this;
	}
	
	int Item::getWeight() const {
		return this->weight;
	}

	bool Item::isDropable() const {
		return dropable;
	}

	Item* Item::setDropable(bool dropable) {
		this->dropable = dropable;
		return this;
	}

	bool Item::isPickable() const {
		return pickable;
	}

	Item* Item::setPickable(bool pickable) {
		this->pickable = pickable;
		return this;
	}

	void Item::getActions(ActionList* list, ObjectPointer callee) {
		IDescriptable::getActions(list, callee);
	}
	
	Item* Item::respawnEvery(int seconds) {
		ObjectPointer item (this);
		item.assertExists("Item must exist to be respawnable");
		ObjectPointer location = getSingleRelation(R_INSIDE, Relation::Slave);
		if (!location)
			throw new GameException("Item must be inside something to be respawnable.");
		
		ItemRespawner* trap = new ItemRespawner("trap/respawn/" + getId());
		trap->setInterval(seconds);
		this->getGameManager()->insertObject(trap);
		trap->setTemplate(item);
		location->attachTrap(trap, "get-actions");
		this->getGameManager()->removeRelation(location, this, R_INSIDE);
		return this;
	}

	
	string Item::getDescription() const {
		stringstream ss;
		ss << IDescriptable::getDescription();
		if (getWeight())
			ss << "It weights " << Utils::weightStr(getWeight()) << ".";
		else ss << "It weights almost nothing."; 
		if (!isPickable())
			ss << "It cannot be taken.";
		if (!isDropable())
			ss << "Once taken, it cannot be dropped.";
		return ss.str();
	}

	
	string Item::getDescriptionSentence() {
		return getGroupDescriptionSentence({ this });
	}
	
	string Item::getGroupDescriptionSentence(vector<ObjectPointer> others) {
		SentenceJoiner items;
		for (auto i = others.begin(); i != others.end(); i++) {
			items << *i;
		}

		// TODO we need way more nice sentences cause this is displaying veery often.
		return RandomString::get()
				<< items.getSentence("", "You see % lying there.") << endr
				<< items.getSentence("", "There lies %." ) << endr
				<< items.getSentence("", "% lies there.") << endr
				<< items.getSentence("", "There is also some %.", "There are % too.") << endr
				<< items.getSentence("", "There is %.", "There are %.") << endr;
	}
	
	void Item::registerProperties(IPropertyStorage& storage) {
		storage.have(size, "item-size", "Item size")
			.have(weight, "item-weight", "Item weight")
			.have(pickable, "item-pickable", "Item Pick-ability")
			.have(dropable, "item-dropable", "Item Drop-ability");
		IDescriptable::registerProperties(storage);
	}
	
	PERSISTENT_IMPLEMENTATION(Item)



}
