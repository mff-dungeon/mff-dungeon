#include "Dropper.hpp"
#include "../../Persistence/Cloner.hpp"
#include "../../Game/GameManager.hpp"
#include "../Resource.hpp"
#include "../Location.hpp"

namespace Dungeon {

	Dropper::Dropper() { }

	Dropper::Dropper(const objId& id) : Base(id) { }

	Dropper::~Dropper() { }

	ObjectPointer Dropper::getItem() {
		return getSingleRelation("dropper-item", Relation::Master);
	}

	Dropper* Dropper::setItem(ObjectPointer item) {
		setSingleRelation("dropper-item", item, Relation::Master);
		return this;
	}

	int Dropper::getChance() const {
		return chance;
	}

	Dropper* Dropper::setChance(int chance) {
		this->chance = chance;
		return this;
	}

	int Dropper::getMax() const {
		return max;
	}

	Dropper* Dropper::setMax(int max) {
		this->max = max;
		return this;
	}

	int Dropper::getMin() const {
		return min;
	}

	Dropper* Dropper::setMin(int min) {
		this->min = min;
		return this;
	}

	bool Dropper::tryDrop(ObjectPointer loc, SentenceJoiner& info) {
		loc.assertExists("You cannot drop item nowhere.").assertType<Location>("You must drop items in the room.");
		Location* l = loc.unsafeCast<Location>();
		LOGS(Debug) << "Using " << getId() << " to calculate drop." << LOGF;
		int random = Utils::getRandomInt(0.0_p, 100.0_p);
		if (random <= getChance()) { // Let's drop it
			string itemName;
			int amount = Utils::getRandomInt(getMin(), getMax());
			if (getItem()->instanceOf(Resource)) {
				ObjectPointer item = Cloner::deepClone(getItem());
				getGameManager()->clearRelationsOfType(item, "dropper-item", Relation::Slave);
				itemName = Resource::ResourceName[getItem().unsafeCast<Resource>()->getType()];
				item.safeCast<Resource>()->setQuantity(amount);
				l->addItem(item);
			} else {
				for (int i = 1; i <= amount; i++) {
					ObjectPointer item = Cloner::deepClone(getItem());
					getGameManager()->clearRelationsOfType(item, "dropper-item", Relation::Slave);
					l->addItem(item);
				}
				itemName = getItem().safeCast<IDescriptable>()->getName();
			}
			info << (amount > 1 ? to_string(amount) + " " + itemName : itemName);
			LOGS(Debug) << "Dropped " << amount << " items of type " << itemName << "." << LOGF;
			return true;
		}
		return false;
	}

	void Dropper::registerProperties(IPropertyStorage& storage) {
		storage.have(chance, "dropper-chance", "Chance of the drop")
				.have(min, "dropper-min", "Minimum amount of dropped items")
				.have(max, "dropper-max", "Maximum amount of dropped items");
		
		Base::registerProperties(storage);
	}

	PERSISTENT_IMPLEMENTATION(Dropper)
}

