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

	bool Dropper::tryDrop(ObjectPointer loc) {
		loc.assertExists("You cannot drop item nowhere.").assertType<Location>("You must drop items in the room.");
		int random = Utils::getRandomInt(1, 1000000);
		if (random <= getChance()) { // Let's drop it
			int amount = Utils::getRandomInt(getMin(), getMax());
			if (getItem()->isInstanceOf(Resource::ResourceClassName)) {
				ObjectPointer item = Cloner::deepClone(getItem());
				getGameManager()->clearRelationsOfType(item, "dropper-item", Relation::Slave);
				item.safeCast<Resource>()->setQuantity(amount)->attachSumTrap()->save();
				item->setSingleRelation(R_INSIDE, loc, Relation::Slave);
			} else {
				for (int i = 1; i <= amount; i++) {
					ObjectPointer item = Cloner::deepClone(getItem());
					getGameManager()->clearRelationsOfType(item, "dropper-item", Relation::Slave);
					item->setSingleRelation(R_INSIDE, loc, Relation::Slave);
				}
			}
			string itemName = getItem().safeCast<IDescriptable>()->getName();
			LOGS(Debug) << "Dropped " << amount
					<< " items of type " << itemName << "." << LOGF;
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

	void Dropper::getActions(ActionList* list, ObjectPointer callee) { }

	PERSISTENT_IMPLEMENTATION(Dropper)
}

