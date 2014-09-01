#include <cstring>

#include "Item.hpp"
#include "../Actions/CallbackAction.hpp"
#include "../ActionDescriptor.hpp"

namespace Dungeon {
	
	Item::Item() {
		this->setDropable(true)->setPickable(true)->setSize(0)->setWeight(0);
	}
	
	Item::Item(objId id) {
		this->setId(id);
		this->setDropable(true)->setPickable(true)->setSize(0)->setWeight(0);
	}

	Item::~Item() {

	}
	
	Item* Item::setSize(int size) {
		this->size = size;
		return this;
	}
	
	int Item::getSize() {
		return this->size;
	}
	
	Item* Item::setWeight(int weight) {
		this->weight = weight;
		return this;
	}
	
	int Item::getWeight() {
		return this->weight;
	}

	bool Item::isDropable() {
		return dropable;
	}

	Item* Item::setDropable(bool dropable) {
		this->dropable = dropable;
		return this;
	}

	bool Item::isPickable() {
		return pickable;
	}

	Item* Item::setPickable(bool pickable) {
		this->pickable = pickable;
		return this;
	}

	void Item::getActions(ActionList* list, IObject* callee) {
		
	}
	
	string Item::getDescriptionSentence() {
		rand_init(gen, dist, 0, 1);
        int rnd = rand_next(gen, dist);
        
        switch (rnd) {
            case 0:
				return "You see " + this->getName() + " lying on the ground.";
            case 1:
                return "There lies " + this->getName() + ".";
            default:
                return this->getName() + " is nearby.";
        }
	}
	
	string Item::getGroupDescriptionSentence(vector<IDescriptable*> others) {
		if (others.size() == 0) return "";
        else if (others.size() == 1) return getDescriptionSentence();
        
        string sentence;
        rand_init(gen, dist, 0, 1);
        int rnd = rand_next(gen, dist);
        
        switch (rnd) {
            case 0:
                sentence = "You see ";
                break;
            case 1:
                sentence = "There lies ";
                break;
            default:
                sentence = "";
                break;
        }
        
        for (int i = 0; i < others.size() - 1; i++) {
            if (i != 0) sentence += ", ";
            sentence += others.at(i)->getName();
        }
        
        sentence += " and " + others.at(others.size() - 1)->getName();
        
        switch (rnd) {
            case 1:
                sentence += " on the ground.";
                break;
            default:
                sentence += ".";
                break;
        }
        
        return sentence;
	}
	
	void Item::registerProperties(IPropertyStorage& storage) {
		storage.have(size, "Item size").have(weight, "Item weight");
		IDescriptable::registerProperties(storage);
	}
	
	PERSISTENT_IMPLEMENTATION(Item)
}
