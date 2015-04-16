#include "Resource.hpp"
#include "../Game/ActionDescriptor.hpp"
#include "../Game/GameManager.hpp"
#include "Alive.hpp"
#include "../Persistence/Cloner.hpp"

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
		save();
		return this;
	}

	ObjectPointer Resource::split(int secondAmount) {
		if(secondAmount <= 0 || secondAmount >= getQuantity()) return nullptr;
		ObjectPointer newResPtr = Cloner::shallowClone(this);
		this->setQuantity(getQuantity() - secondAmount);
		newResPtr.unsafeCast<Resource>()->setQuantity(secondAmount);
		LOGS(Debug) << "Split " << Resource::ResourceName[getType()] 
				<< " into 2 parts (" << getQuantity() << ", " << secondAmount << ")." << LOGF;
		return newResPtr;
	}
	
	ObjectPointer Resource::join(ObjectPointer other) {
		other.assertType<Resource>();
		LOGS(Debug) << "Joining " << Resource::ResourceName[getType()] 
				<< " (" << getQuantity() << ", " << other.unsafeCast<Resource>()->getQuantity() << ")." << LOGF;
		setQuantity(getQuantity() + other.unsafeCast<Resource>()->getQuantity());
		getGameManager()->deleteObject(other);
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
		Item::onPick(ad);
		return this;
	}
	
	ObjectPointer Resource::onDrop(ActionDescriptor* ad) {
		Item::onDrop(ad);
		return this;
	}

	PERSISTENT_IMPLEMENTATION(Resource)
}
