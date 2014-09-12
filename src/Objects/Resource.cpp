#include "Resource.hpp"
#include "Alive.hpp"

namespace Dungeon {

	Resource::ResourceType Resource::getType() const {
		return resourceType;
	}
	
	Resource* Resource::setType(Resource::ResourceType resourceType) {
		this->resourceType = resourceType;
		return this;
	}

	int Resource::getQuantity() const {
		return quantity;
	}
	
	Resource* Resource::setQuantity(int quantity) {
		this->quantity = quantity;
		return this;
	}
        
        string Resource::getName() const {
            switch (getType()) {
                case ResourceType::Gold:
                    return "Gold";
                case ResourceType::Wood:
                    return "Wood";
                case ResourceType::Stone:
                    return "Stone";
                case ResourceType::Food:
                    return "Food";
                case ResourceType::ManaShard:
                    return "Mana shard";
                default:
                    // no laughing, this is actually a serious error
                    return "Puppies";
            }
        }
        
        string Resource::getLongName() const {
            switch (getType()) {
                case ResourceType::Gold:
                    return "A chunk of great wealth (" + to_string(getQuantity()) + " units).";
                case ResourceType::Wood:
                    return "Smells like forest (" + to_string(getQuantity()) + " units).";
                case ResourceType::Stone:
                    return "It is a bit heavier than it looks (" + to_string(getQuantity()) + " units).";
                case ResourceType::Food:
                    return "Tasty and nutricious at the same time (" + to_string(getQuantity()) + " units).";
                case ResourceType::ManaShard:
                    return "It's a kind of magic (" + to_string(getQuantity()) + " units).";
                default:
                    // no laughing, this is actually a serious error
                    return "They are more adorable than you would believe.";
            }
        }
	
	string Resource::getDescription() const {
            return "Yay a description!";
	}

	void Resource::registerProperties(IPropertyStorage& storage) {
		storage.have((int&) resourceType, "resource-type", "Type of the resource (0 - gold, 1 - wood, 2 - stone, 3 - food, 4 - mana shard)");
		storage.have(quantity, "resource-quantity", "Quantity of the resource");
		Item::registerProperties(storage);
	}
        
        Resource* Resource::attachSumTrap() {
            GameManager* gm = getGameManager();
            ResourceSumTrap* trap = new ResourceSumTrap(this->getId() + "/sumtrap");
            
            gm->insertObject(trap);
            trap->save();
            
            this->attachTrap(trap, "picked");
            return this;
        }
        
        void ResourceSumTrap::trigger(string event, ObjectPointer target, ActionDescriptor* ad) {
            target.assertType<Resource>("Tried to sum non-resource.");
            
            Resource* newResource = target.safeCast<Resource>();            
            Alive* sender = ad->getAlive();
            
            *ad << " sum trap! gold before: " << sender->getResourceQuantity(Resource::ResourceType::Gold);            
            sender->addResource(newResource);
            *ad << " gold after: " << sender->getResourceQuantity(Resource::ResourceType::Gold);
	}
	
	PERSISTENT_IMPLEMENTATION(ResourceSumTrap)
	
	PERSISTENT_IMPLEMENTATION(Resource)
}
