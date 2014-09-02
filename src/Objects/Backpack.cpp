
#include "Backpack.hpp"
#include "Alive.hpp"
#include "../RandomString.hpp"

namespace Dungeon {
	Backpack::Backpack() {
		this->maxSpace = DEFAULT_SPACE;
		this->maxWeight = DEFAULT_WEIGHT;
		this->usedSpace = 0;
		this->usedWeight = 0;
	}
	
	Backpack::Backpack(objId id) {
		this->setId(id);
		this->maxSpace = DEFAULT_SPACE;
		this->maxWeight = DEFAULT_WEIGHT;
		this->usedSpace = 0;
		this->usedWeight = 0;
	}

	Backpack::~Backpack() {

	}
	
	void Backpack::setMaxSpace(int maxSpace) {
		this->maxSpace = maxSpace;
	}
	
	int Backpack::getMaxSpace() {
		return this->maxSpace;
	}
	
	int Backpack::getFreeSpace() {
		return (this->maxSpace - this->usedSpace);
	}
	
	void Backpack::setMaxWeight(int maxWeight) {
		this->maxWeight = maxWeight;
	}
	
	int Backpack::getMaxWeight() {
		return this->maxWeight;
	}
	
	int Backpack::getFreeWeight() {
		return (this->maxWeight - this->usedWeight);
	}
	
	void Backpack::addItem(Item* item) {
		this->usedWeight += item->getWeight();
		this->usedSpace += item->getSize();
		this->getGameManager()->createRelation(this, item, R_INVENTORY);
		save();
	}
	
	void Backpack::removeItem(Item* item) {
		this->usedWeight -= item->getWeight();
		this->usedSpace -= item->getSize();
		this->getGameManager()->removeRelation(this, item, R_INVENTORY);
		save();
	}
	
	string Backpack::getDescriptionSentence() {
		string sentence;
        
		sentence = RandomString::get()
			<< "You own a " + this->getName() + "." << endr
            << "You have a " + this->getName() + " with you." << endr;
		
		try {
			ObjectMap items = this->getRelations(true).at(R_INVENTORY);
			if(items.size() >= 2) {
				int count = 0;
				sentence += " There are ";
				for(auto& item : items) {
					if(count > 0 && count < items.size() - 1) {
						sentence += ", ";
					}
					else if(count == items.size() - 1) {
						sentence += " and ";
					}
					count++;
					if(item.second.get()->instanceOf(IDescriptable)) {
						IDescriptable* itemDesc = (IDescriptable*) item.second.get();
						sentence += itemDesc->getName();
					}
					else {
						sentence += item.second.getId();
					}
				}
				sentence += ".";
			}
			else if(items.size() == 1) {
				for(auto& item : items) {
					sentence += " There is ";
					if(item.second.get()->instanceOf(IDescriptable)) {
						IDescriptable* itemDesc = (IDescriptable*) item.second.get();
						sentence += itemDesc->getName();
					}
					else {
						sentence += item.second.getId();
					}
					sentence += ".";
				}
			}
		}
		catch(const std::out_of_range& e) {
			
		}
		return sentence;
	}
	
	string Backpack::getGroupDescriptionSentence(vector<IDescriptable*> others) {
		if (others.size() == 0) return "";
        else if (others.size() == 1) return getDescriptionSentence();
        
        string sentence = "";
        
		//TODO: Write this
        
        return sentence;
	}
	
	void Backpack::getActions(ActionList* list, IObject* callee) {
		Item::getActions(list, callee);
		
		// Drop action, adding only if there is anything to drop
		DropAction* action = new DropAction;
		try {
			ObjectMap itemsIn = this->getRelations(true).at(R_INVENTORY);
			for(auto& item : itemsIn) {
				item.second.get()->getActions(list, callee);
				action->addTarget(item.second);
			}
		}
		catch (const std::out_of_range& e) {
			
		}
		if(action->getTargets().size() > 0) {
			list->addAction(action);
		}
	}
	
	void Backpack::registerProperties(IPropertyStorage& storage) {
		storage.have(maxSpace, "Maximum space of backpack").
				have(maxWeight, "Maximum weight of backpack").
				have(usedSpace, "Space currently used").
				have(usedWeight, "Weight currently used");
		Item::registerProperties(storage);
	}
	
	PERSISTENT_IMPLEMENTATION(Backpack)
	
	void DropAction::explain(ActionDescriptor* ad) {
		*ad << "Use 'drop ...' to drop items from your backpack. \n";
	}
	
	bool DropAction::matchCommand(string command) {
		return RegexMatcher::match("drop .+", command);
	}
        
	void DropAction::commit(ActionDescriptor* ad) {	
		commitOnBestTarget(ad, ad->in_msg.substr(5));
	}
        
	void DropAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {	
		// Presumes the target is instance of item
		Item* item = (Item*) target.get();
		
		if(!item->isDropable()) {
			*ad << "You cannot drop this item.\n";
			return;
		}
		
		// Get the backpack
		Backpack* backpack;
		try {
			ObjectMap backpacks = item->getRelations(false).at(R_INVENTORY);
			if(backpacks.size() == 0) {
				*ad << "You cannot drop this item.\n";
				return;
			}
			backpack = (Backpack*) backpacks.begin()->second.get();
		}
		catch (const std::out_of_range& e) {
			
		}
		
		backpack->removeItem(item);
		ad->getGM()->createRelation(ad->getAlive()->getLocation().get(), item, R_INSIDE);
		
		*ad << "You've dropped " + item->getName() + ".";
	}
}