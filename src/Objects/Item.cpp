#include <cstring>
#include <stdexcept>

#include "Item.hpp"
#include "../Utils/RandomString.hpp"
#include "../Utils/SentenceJoiner.hpp"
#include "../Actions/CallbackAction.hpp"
#include "../Game/ActionDescriptor.hpp"
#include "../Game/GameManager.hpp"
#include "Traps/ItemRespawner.hpp"
#include "Virtual/Stats.hpp"
#include "Human.hpp"

namespace Dungeon {

	Item::Item() {
		this->setDropable(true)->setPickable(true)->setSize(0)->setWeight(0);
	}

	Item::Item(const objId& id) : IDescriptable(id) {
		this->setDropable(true)->setPickable(true)->setSize(0)->setWeight(0);
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
		// Intentionally left blank?
	}

	Item* Item::addStatReq(ObjectPointer reqPtr) {
		reqPtr.assertExists("The requirement doesn't exist.")
				.assertType<StatReq>("You can only add a stat requirement");
		StatReq* req = reqPtr.unsafeCast<StatReq>();
		getGameManager()->createRelation(this, req, R_REQUIREMENT);
		return this;
	}

	bool Item::checkStatReqs(ObjectPointer userPtr, ActionDescriptor* ad) {
		userPtr.assertExists("The human is weird.")
				.assertType<Human>("Only a human has stats.");
		Human* user = userPtr.unsafeCast<Human>();
		try {
			const ObjectMap& reqs = getRelations(Relation::Master, R_REQUIREMENT);
			for (auto& req : reqs) {
				req.second.assertType<StatReq>("Only a requirement is supposed to be there.");
				StatReq* r = req.second.unsafeCast<StatReq>();
				if (user->getStatValue(r->getStat()) < r->getValue()) {
					*ad << "Your " << user->getStatName(r->getStat()) << " is not high enough." << eos;
					return false;
				}
			}
		} catch (std::out_of_range& e) {

		}
		return true;
	}

	Item* Item::respawnEvery(int seconds) {
		ObjectPointer item(this);
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
		return getGroupDescriptionSentence({this});
	}

	string Item::getGroupDescriptionSentence(vector<ObjectPointer> others) {
		SentenceJoiner items;
		for (auto i = others.begin(); i != others.end(); i++) {
			items << *i;
		}

		// TODO we need way more nice sentences cause this is displaying very often.
		return RandomString::get()
				<< items.getSentence("", "You see % lying there.") << endr
				<< items.getSentence("", "There lies %.") << endr
				<< items.getSentence("", "% lies there.") << endr
				<< items.getSentence("", "There is also some %.", "There is % too.") << endr
				<< items.getSentence("", "There is %.", "There is %.") << endr;
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
