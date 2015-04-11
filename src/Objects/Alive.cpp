#include <stdexcept>
#include <math.h>
#include "../Utils/RandomString.hpp"
#include "../Utils/SentenceJoiner.hpp"
#include "../Game/ActionDescriptor.hpp"
#include "../Game/GameManager.hpp"
#include "../Game/ActionList.hpp"
#include "Traps/Healing.hpp"
#include "Inventory.hpp"
#include "Alive.hpp"
#include "Location.hpp"
#include "Wearable.hpp"

namespace Dungeon {

	/*************************************
	 *	HUGE FIXME
	 *		- move this method to Human - alive shouldn't call actions in general
	 **************************************/
	void Alive::getAllActions(ActionDescriptor* ad){
		LOGS(Debug) << "Getting all actions for " << this->getId() << "." << LOGF;
		triggerTraps("get-all-actions", ad);

		LOGS(Debug) << "Getting actions in location - " << this->getId() << "." << LOGF;
		auto room = getSingleRelation(R_INSIDE, Relation::Slave);
		if (!room)
			this->getActionsRecursive(ad);
		else // Adding actions on ourselves will be delegated.
			room->getActionsRecursive(ad);
	}

	void Alive::getActions(ActionDescriptor* ad) {
		if (getState() == State::Dead) {
			LOGS(Debug) << this->getId() << " is dead." << LOGF; 
			return;
		}
		
		if (this == ad->getCaller()) { // Remove this condition to allow stealing ;)
			LOGS(Debug) << "Getting actions on inventory of " << this->getId() << "." << LOGF;
			delegateGetActions(ad, { R_INVENTORY, "special-th" });
			for (int slot = 1; slot < Wearable::Slot::Count; slot++)
				delegateGetActions(ad, Wearable::SlotRelations[slot]);
		}
	}

	void Alive::registerProperties(IPropertyStorage& storage) {
		storage.have(currentHp, "alive-currenthp", "Current hitpoints")
				.have(maxHp, "alive-maxhp", "Maximum hitpoints")
				.have(attack, "alive-attack", "Attack value")
				.have(defense, "alive-defense", "Defense value")
				.have((int&) currentState, "alive-state", "Current state of alive: 1 - Living, 2 - Dying, 3 - Dead")
				.have(respawnTime, "alive-respawntime", "Respawn time")
				.have(respawnInterval, "alive-respawninterval", "Respawn interval")
				.have(weaponName, "alive-weaponname", "The name of weapon currently used");
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
		if (ad != nullptr && this->attack != attack && ad->getCaller() == this) {
			*ad << "Your attack value has changed to " + to_string(attack) + "." << eos;
		}
		this->attack = attack;
		return this;
	}

	int Alive::getDefense() const {
		return defense;
	}

	Alive* Alive::setDefense(int defense, ActionDescriptor* ad) {
		if (ad != nullptr && this->defense != defense && ad->getCaller() == this) {
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

		if (ad != nullptr && this->currentHp != hp && ad->getCaller() == this) {
			*ad << "Your current hitpoints have changed to " + to_string(hp) + "." << eos;
		}
		this->currentHp = hp;
		LOGS(Debug) << "Hitpoints of " << getId() << " were changed to " << hp << "." << LOGF;
		return this;
	}

	int Alive::getMaxHp() const {
		return maxHp;
	}

	Alive* Alive::setMaxHp(int hp, ActionDescriptor* ad) {
		if (ad != nullptr && this->maxHp != maxHp && ad->getCaller() == this) {
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

	bool Alive::hasItemType(const string& type) {
		ObjectPointer backpack = getBackpack();
		if (!backpack) return false;
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

	int Alive::calculateDamage(ObjectPointer attackerPtr, int amount) {
		Alive* attacker = attackerPtr.safeCast<Alive>();
		if (attacker == 0) return 0;
		double multiplier = 10;
		if (getDefense() != 0) { // Division by zero
			multiplier = (double) sqrt(amount / getDefense());
			if (multiplier > 10) multiplier = 10;
		}

		int rnd = Utils::getRandomInt(90, 110);

		int damage = (int) (multiplier * rnd / 100 * (amount - getDefense() * 0.2));
		if (damage <= 0) return 0;
		return damage;
	}

	Alive* Alive::damageAlive(ObjectPointer attackerPtr, int amount, ActionDescriptor* ad) {
		LOGS(Debug) << "Damaging alive " << attackerPtr.getId() << "." << LOGF;
		Alive* attacker = attackerPtr.safeCast<Alive>();
		if (amount <= 0) return this;
		if (ad != nullptr) {
			if (ad->getCaller() == this) {
				if (attacker->getWeaponName() != "")
					*ad << (RandomString::get()
						<< "You have received " << amount << " damage from " + attacker->getName() + "'s " + Utils::decapitalize(attacker->getWeaponName()) + "." << endr
						<< attacker->getName() << " caused you " << amount << " damage with its " << Utils::decapitalize(attacker->getWeaponName()) << "." << endr) << eos;
				else
					*ad << "You have received " + to_string(amount) + " damage from " + attacker->getName() + "." << eos;
			} else {
				string weapon = ad->getCaller()->getWeaponName();
				if (weapon != "")
					*ad << (RandomString::get()
						<< "You have dealt " << amount << " damage to " + this->getName() + " with your " + Utils::decapitalize(move(weapon)) + "." << endr
						<< "Your " + Utils::decapitalize(move(weapon)) + " has caused " << amount << " damage to " + this->getName() + "." << endr) << eos;
				else
					*ad << "You have dealt " << amount << " damage to " + this->getName() + "." << eos;
			}
		}
		changeHp(-amount, ad);
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
			if (quantity != 0) {
				res->setQuantity(quantity);
				res->save();
			} else {
				getGameManager()->deleteObject(current);
			}
		} else if (quantity != 0) {
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
		LOGS(Debug) << "Attached healing trap to " << getId() << " healing at rate " << rate << "." << LOGF;
		Healing* heal = new Healing("trap/healing/" + getId());
		heal->setRate(rate);
		getGameManager()->insertObject(heal);

		heal->setTarget(this);
		return this;
	}

	PERSISTENT_IMPLEMENTATION(Alive)
}

