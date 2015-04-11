#include "Resource.hpp"
#include "../Game/ActionDescriptor.hpp"
#include "../Game/GameManager.hpp"
#include "Alive.hpp"

namespace Dungeon {

	const char* Resource::ResourceName[] = {"gold", "wood", "iron", "dragon skin",
		"leather", "sand", "white powder", "red powder", "magical stone", "mana shards"};

	const char* Resource::ResourceIdentifier[] = {"gold", "wood", "iron", "dragon-skin",
		"leather", "sand", "white-powder", "red-powder", "magical-stone", "mana-shards"};
	
	objId Resource::getResourceTypeId(ResourceType type) const {
		return "resource/" + (string) ResourceIdentifier[(int) type] + "/" + RANDID;
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
		return to_string(getQuantity()) + " " + ResourceName[(int) getType()];
	}

	string Resource::getLongName() const {
		return to_string(getQuantity()) + " units of " + ResourceName[(int) getType()];
	}

	int Resource::getSize() const {
		return Item::getSize() * getQuantity();
	}

	Item* Resource::setSize(int size) {
		return Item::setSize(size);
	}

	int Resource::getWeight() const {
		return Item::getWeight() * getQuantity();
	}

	Item* Resource::setWeight(int weight) {
		return Item::setWeight(weight);
	}

	string Resource::getDescription() const {
		switch (getType()) {
			case ResourceType::Gold:
				return "A chunk of great wealth (" + to_string(getQuantity()) + " units).";
			case ResourceType::Wood:
				return "Smells like forest (" + to_string(getQuantity()) + " units).";
			case ResourceType::DragonSkin:
				return "Rare skin of a dragon (" + to_string(getQuantity()) + " units).";
			case ResourceType::Iron:
				return "Some raw iron ore (" + to_string(getQuantity()) + " units).";
			case ResourceType::Leather:
				return "Some leather from animals (" + to_string(getQuantity()) + " units).";
			case ResourceType::MagicalStone:
				return "Shining magical stones (" + to_string(getQuantity()) + " units).";
			case ResourceType::ManaShard:
				return "It's a kind of magic (" + to_string(getQuantity()) + " units).";
			case ResourceType::RedPowder:
				return "Looks like some magical powder (" + to_string(getQuantity()) + " units).";
			case ResourceType::WhitePowder:
				return "Seems like an ordinary powder (" + to_string(getQuantity()) + " units).";
			case ResourceType::Sand:
				return "Stuff from beaches (" + to_string(getQuantity()) + " units).";
			default:
				LOGS(Error) << "Invalid resource type requested " << getType() << "." << LOGF;
				return "They are more adorable than you would believe.";
		}
	}

	void Resource::registerProperties(IPropertyStorage& storage) {
		storage.have((int&) resourceType, "resource-type", "Type of the resource");
		storage.have(quantity, "resource-quantity", "Quantity of the resource");
		Item::registerProperties(storage);
	}
	
	ObjectPointer Resource::onPick(ActionDescriptor* ad) {
		string rel = R_RESOURCE(resourceType);
		Item::onPick(ad);
		ad->getCaller()->addResource(this);
		return ad->getCaller()->getSingleRelation(rel, Relation::Master);
	}
	
	ObjectPointer Resource::onDrop(ActionDescriptor* ad) {
		Item::onDrop(ad);
		ad->getCaller()->setSingleRelation(R_RESOURCE(resourceType), nullptr, Relation::Master);
		return this;
	}

	PERSISTENT_IMPLEMENTATION(Resource)
}
