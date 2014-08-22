#include "Item.hpp"

namespace Dungeon {
	
	Potion::Potion() {

	}
	
	Potion::Potion(objId id) : Item(id) {
		
	}

	Potion::~Potion() {

	}
	
	Potion* Potion::setPotionType(PotionType type) {
		this->type = type;
		return this;
	}
	
	Potion* Potion::setStrength(int strength) {
		this->strength = strength;
		return this;
	}
	
	void Potion::getActions(ActionList* list, IObject* callee) {
		Item::getActions(list, callee);
		// Check if the callee hasn't this item and it lies in the current room
		// FIX ME - find better way to check, if we can cast callee - we need its location
		try{
			if(strcmp(callee->className().c_str(),"Alive") == 0 || strcmp(callee->className().c_str(), "Human") == 0) {
				ObjectMap objectsInv = getRelations(false).at(R_INVENTORY);
				if(objectsInv.find(callee->getId()) != objectsInv.end()) {
					//Great, let's drink it
					string match = "Drink " + this->getName();
					transform(match.begin(), match.end(), match.begin(), ::tolower);
					list->addAction(new CallbackAction("drink", "Drink " + this->getName(), 
						RegexMatcher::matcher(match),
						[this, callee] (ActionDescriptor * ad) {
							*ad << "You've drunk " + this->getName() + ". ";
							switch(type) {
								case PotionType::Healing:
									((Alive*) callee)->hitpoints += strength;
									*ad << "You've healed " + to_string(strength) + " hitpoints";
									break;
								case PotionType::NoEffect:
								default:
									*ad << "... and it did nothing.";
							}
							ad->getGM()->removeRelation(callee, this, R_INVENTORY);
							ad->getGM()->deleteObject(this);
						}));
				}
			}
		} catch (const std::out_of_range& e) {
			
		}
	}
	
	void Potion::serialize(Archiver& stream) {
		if (stream.isStoring()) {
			stream << type;
			stream << strength;
		} else {
			int t;
			// To deal with enum
			stream >> t;
			stream >> strength;
			this->setPotionType((PotionType) t);
		}
		Item::serialize(stream);
	}

	PERSISTENT_IMPLEMENTATION(Potion)
}