#include "Resource.hpp"
#include "Alive.hpp"

namespace Dungeon {
    
    string Resource::getResourceTypeName(ResourceType type) const {
        switch (type) {
            case ResourceType::Gold:
                return "gold";
            case ResourceType::Wood:
                return "wood";
            case ResourceType::Stone:
                return "stone";
            case ResourceType::Food:
                return "food";
            case ResourceType::ManaShard:
                return "manashard";
            default:
                // no laughing, this is actually a serious error
                return "puppies";
        }
    }
    
    objId Resource::getResourceTypeId(ResourceType type) const {
        return "resource/" + getResourceTypeName(type) + "/" + RANDID;
    }
    
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
        return getResourceTypeName(getType());
    }

    string Resource::getLongName() const {
        return to_string(getQuantity()) + " " + getResourceTypeName(getType());
    }

    string Resource::getDescription() const {
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
            
        sender->addResource(newResource);
    }

    PERSISTENT_IMPLEMENTATION(ResourceSumTrap)

    PERSISTENT_IMPLEMENTATION(Resource)
}
