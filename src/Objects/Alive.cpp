#include <stdexcept>
#include <math.h>
#include "Alive.hpp"
#include "Location.hpp"
#include "Wearable.hpp"
#include "../ActionDescriptor.hpp"
#include "../ActionList.hpp"
#include "../RandomString.hpp"
#include "../SentenceJoiner.hpp"
#include "../Traps/Healing.hpp"
#include "Inventory.hpp"

namespace Dungeon {

	Alive::Alive(objId id) : IDescriptable(id) { }

	void Alive::getAllActions(ActionList* list) {
		LOGS("Alive", Verbose) << "Getting all actions on " << this->getId() << "." << LOGF;

		triggerTraps("get-all-actions", nullptr);

		// Add some actions on myself
		this->getActions(list, this);
		if (getState() == State::Dead) {
			// No interacting!
			return;
		}

		// Get actions for the inventory items - thors hammer
		LOGS("Alive", Verbose) << "Getting actions on inventory - " << this->getId() << "." << LOGF;
		try {
			RelationList mastering = getRelations(Relation::Master);
			for (auto& pair : mastering)
				for (auto& item : pair.second) {
					LOGS("Alive", Verbose) << "Getting actions " << item.second << "." << LOGF;
					if (item.second->instanceOf(Item))
						item.second->triggerTraps("get-actions", nullptr)
						->getActions(list, this);
					else
						LOGS("Alive", Verbose) << item.second << " is not item" << LOGF;
				}
		} catch (const std::out_of_range& e) {
			// Nothing needs to be done
		}

		LOGS("Alive", Verbose) << "Getting actions on equiped items" << LOGF;
		for (int i = Wearable::Slot::BodyArmor; i != Wearable::Slot::Invalid; i--) {
			ObjectPointer equip = getSingleRelation(Wearable::SlotRelations[i], Relation::Master, GameStateInvalid::EquippedMoreThanOne);
			if (!!equip) {
				equip->triggerTraps("get-actions", nullptr)
						->getActions(list, this);
			}
		}

		LOGS("Alive", Verbose) << "Getting actions in location - " << this->getId() << "." << LOGF;
		// Find objects in current location
		try {
			ObjectMap room = getRelations(Relation::Slave, R_INSIDE);
			for (auto& item : room) {
				item.second->triggerTraps("get-actions", nullptr)
						->getActions(list, this);
			}
		} catch (const std::out_of_range& e) {
			// Nothing needs to be done
		}
	}

	void Alive::getActions(ActionList* list, ObjectPointer callee) {
		triggerTraps("get-actions", nullptr);
		IDescriptable::getActions(list, callee);
		// Add actions if any will be generic to all alives
	}

	void Alive::registerProperties(IPropertyStorage& storage) {
		storage.have(currentHp, "alive-currenthp", "Current hitpoints")
				.have(maxHp, "alive-maxhp", "Maximum hitpoints")
				.have(attack, "alive-attack", "Attack value")
				.have(defense, "alive-defense", "Defense value")
				.have((int&) currentState, "alive-state", "Current state of alive: 1 - Living, 2 - Dying, 3 - Dead")
				.have(respawnTime, "alive-respawntime", "Respawn time")
				.have(respawnInterval, "alive-respawninterval", "Respawn interval")
				.have(weaponName, "alive-weaponname", "The name of weapon currently used")
				.have(lastInteraction, "alive-last-interaction", "UNIX timestamp of last interaction");
		IDescriptable::registerProperties(storage);
	}

	string Alive::getDescriptionSentence() {
		return getGroupDescriptionSentence({this});
	}

	string Alive::getGroupDescriptionSentence(vector<ObjectPointer> others) {
		SentenceJoiner alives;
		for (auto i = others.begin(); i != others.end(); i++) {
			alives << *i;
		}

		return RandomString::get()
				<< alives.getSentence("", "You recognize %'s figure.", "You recognize %.", false) << endr
				<< alives.getSentence("", "% is there with you.", "% are there with you.", false) << endr
				<< alives.getSentence("", "You smell the presence of %.", "You smell the presence of %.", false) << endr
				<< alives.getSentence("", "% is nearby.", "% are nearby.", false) << endr;
	}

	ObjectPointer Alive::getLocation() {
		return getSingleRelation(R_INSIDE, Relation::Slave);
	}

	int Alive::getAttack() const {
		return attack;
	}

	Alive* Alive::setAttack(int attack, ActionDescriptor* ad) {
		if (ad != 0 && this->attack != attack && ad->getCaller() == this) {
			*ad << "Your attack value has changed to " + to_string(attack) + "." << eos;
		}
		this->attack = attack;
		return this;
	}

	int Alive::getDefense() const {
		return defense;
	}

	Alive* Alive::setDefense(int defense, ActionDescriptor* ad) {
		if (ad != 0 && this->defense != defense && ad->getCaller() == this) {
			*ad << "Your defense value has changed to " + to_string(defense) + "." << eos;
		}
		this->defense = defense;
		return this;
	}

	int Alive::getCurrentHp() const {
		return currentHp;
	}

	Alive* Alive::setCurrentHp(int hp, ActionDescriptor* ad) {
		if (hp >= maxHp) hp = maxHp;
		if (hp <= 0) hp = 0;

		if (ad != 0 && this->currentHp != hp && ad->getCaller() == this) {
			*ad << "Your current hitpoints have changed to " + to_string(hp) + "." << eos;
		}
		this->currentHp = hp;
		return this;
	}

	int Alive::getMaxHp() const {
		return maxHp;
	}

	Alive* Alive::setMaxHp(int hp, ActionDescriptor* ad) {
		if (ad != 0 && this->maxHp != maxHp && ad->getCaller() == this) {
			*ad << "Your maximum hitpoints have changed to " + to_string(maxHp) + "." << eos;
		}
		this->maxHp = hp;
		if (getCurrentHp() > maxHp) setCurrentHp(maxHp);
		return this;
	}

	double Alive::getPercentageHp() {
		return ((double) currentHp) / maxHp;
	}

	Alive* Alive::calculateBonuses() {
		return this;
	}

	ObjectPointer Alive::getBackpack() {
		return getSingleRelation(Wearable::SlotRelations[Wearable::Backpack], Relation::Master, "You've somehow equipped more than backpack.");
	}

	bool Alive::hasItemType(string type) {
		ObjectPointer backpack = getBackpack();
		if (backpack == 0) return false;
		if (!backpack->isInstanceOf(Inventory::InventoryClassName)) return false;
		Inventory* inv = backpack.unsafeCast<Inventory>();

		try {
			const ObjectMap& items = inv->getRelations(Relation::Master, R_INVENTORY);
			for (auto& item : items) {
				if (item.second->getObjectType().compare(type) == 0)
					return true;
			}
		} catch (const std::out_of_range& e) {

		}
		return false;
	}

	Alive* Alive::damageAlive(ObjectPointer attackerPtr, int amount, ActionDescriptor* ad) {
		Alive* attacker = attackerPtr.safeCast<Alive>();
		if (attacker == 0) return this;
		double multiplier = 10;
		if (getDefense() != 0) { // Division by zero
			multiplier = (double) sqrt(amount / getDefense());
			if (multiplier > 10) multiplier = 10;
		}

		int rnd = Utils::getRandomInt(90, 110);

		int damage = (int) (multiplier * rnd / 100 * (amount - getDefense() * 0.2));
		if (damage <= 0) return this;
		if (ad != 0) {
			if (ad->getCaller() == this) {
				if (attacker->getWeaponName() != "")
					*ad << (RandomString::get()
						<< "You have received " << damage << " damage from " + attacker->getName() + "'s " + Utils::decapitalize(attacker->getWeaponName()) + "." << endr
						<< attacker->getName() << " caused you " << damage << " damage with its " << Utils::decapitalize(attacker->getWeaponName()) << "." << endr) << eos;
				else
					*ad << "You have received " + to_string(damage) + " damage from " + attacker->getName() + "." << eos;
			} else {
				string weapon = ad->getCaller()->getWeaponName();
				if (weapon != "")
					*ad << (RandomString::get()
						<< "You have dealt " << damage << " damage to " + this->getName() + " with your " + Utils::decapitalize(move(weapon)) + "." << endr
						<< "Your " + Utils::decapitalize(move(weapon)) + " has caused " << damage << " damage to " + this->getName() + "." << endr) << eos;
				else
					*ad << "You have dealt " << damage << " damage to " + this->getName() + "." << eos;
			}
		}
		changeHp(-damage, ad);
		return this;
	}

	Alive* Alive::changeHp(int amount, ActionDescriptor* ad) {
		setCurrentHp(getCurrentHp() + amount, ad);
		if (this->currentHp <= 0) {
			die(ad);
		}
		save();
		return this;
	}

	Alive* Alive::die(ActionDescriptor* ad) {
		// Should be overriden, this one does nothing
		return this;
	}

	Alive* Alive::respawn(ActionDescriptor* ad) {
		// Should be overriden, this one does nothing
		return this;
	}

	ObjectPointer Alive::getRespawnLocation() {
		return getSingleRelation("respawn-location", Relation::Master, "Respawn location not found!");
	}

	Alive* Alive::setRespawnLocation(ObjectPointer room) {
		room.assertType<Location>("Respawn location can only be a room");
		setSingleRelation("respawn-location", room, Relation::Master, "Respawn location couldn't be set.");
		return this;
	}

	int Alive::getRespawnTime() const {
		return respawnTime;
	}

	Alive* Alive::setRespawnTime(int time) {
		this->respawnTime = time;
		return this;
	}

	int Alive::getRespawnInterval() const {
		return respawnInterval;
	}

	Alive* Alive::setRespawnInterval(int interval) {
		this->respawnInterval = interval;
		return this;
	}

	Alive::State Alive::getState() const {
		return currentState;
	}

	Alive* Alive::setState(State newState) {
		this->currentState = newState;
		return this;
	}

	Alive::Presence Alive::getPresence() {
		long unseenTime = (long) time(0) - this->lastInteraction;

		if (unseenTime < 60 * 10) {
			return Presence::Present;
		} else if (unseenTime < 40 * 10) {
			return Presence::Away;
		} else {
			return Presence::Offline;
		}
	}

	Alive* Alive::markInteraction() {
		this->lastInteraction = (long) time(0);
		return this;
	}

	int Alive::getResourceQuantity(Resource::ResourceType type) {
		ObjectPointer resource = getSingleRelation(R_RESOURCE(type), Relation::Master);

		if (!!resource) {
			return resource.unsafeCast<Resource>()->getQuantity();
		} else {
			return 0;
		}
	}

	Alive* Alive::setResourceQuantity(Resource::ResourceType type, int quantity) {
		ObjectPointer current = getSingleRelation(R_RESOURCE(type), Relation::Master);

		if (!!current) {
			Resource* res = current.unsafeCast<Resource>();
			res->setQuantity(quantity);
			res->save();
		} else {
			Resource* res = new Resource(type, quantity);
			GameManager* gm = getGameManager();

			gm->insertObject(res);
			res->save();
			this->setSingleRelation(R_RESOURCE(res->getType()), res);
		}

		return this;
	}

	Alive* Alive::changeResourceQuantity(Resource::ResourceType type, int deltaQuantity) {
		int currentQuantity = this->getResourceQuantity(type);
		this->setResourceQuantity(type, currentQuantity + deltaQuantity);

		return this;
	}

	Alive* Alive::addResource(Resource* resource) {
		ObjectPointer current = getSingleRelation(R_RESOURCE(resource->getType()), Relation::Master);

		if (!!current) {
			Resource* res = current.unsafeCast<Resource>();
			res->setQuantity(res->getQuantity() + resource->getQuantity());
			res->save();

			this->getGameManager()->deleteObject(resource);
		} else {
			this->setSingleRelation(R_RESOURCE(resource->getType()), resource);
		}

		return this;
	}

	Alive* Alive::regenerate(int rate) {
		Healing* heal = new Healing("trap/healing/" + getId());
		heal->setRate(rate);
		getGameManager()->insertObject(heal);

		heal->setTarget(this);
		return this;
	}

	PERSISTENT_IMPLEMENTATION(Alive)
}

