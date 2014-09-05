#include <stdexcept>
#include "Alive.hpp"
#include "Room.hpp"
#include "../ActionDescriptor.hpp"
#include "../ActionList.hpp"
#include "../RandomString.hpp"

namespace Dungeon {
    
	Alive::Alive(objId id) : IDescriptable(id) {
		
	}
	
    void Alive::getAllActions(ActionList* list) {
		LOGS("Alive", Verbose) << "Getting all actions on " << this->getId() << "." << LOGF;
        // Add some actions on myself
        this->getActions(list, this);
		
		// Get actions for the inventory items - thors hammer
		LOGS("Alive", Verbose) << "Getting actions on inventory - " << this->getId() << "." << LOGF;
		try{
			RelationList mastering = getRelations(Relation::Master);
			for (auto& pair : mastering)
				for(auto& item: pair.second) {
					item.second->getActions(list, this);
				}
		}
		catch (const std::out_of_range& e) {
			// Nothing needs to be done
		}
		
		LOGS("Alive", Verbose) << "Getting actions on equiped items" << LOGF;
		for(int i = Wearable::Slot::BodyArmor; i != Wearable::Slot::Invalid; i--) {
			try {
				ObjectMap equips = this->getRelations(Relation::Master, Wearable::SlotRelations[i]);
				if(equips.size() != 0) {
					equips.begin()->second->getActions(list, this);
				}
			}
			catch (const std::out_of_range& e) {
				
			}
		}
		
		LOGS("Alive", Verbose) << "Getting actions in location - " << this->getId() << "." << LOGF;
		// Find objects in current location
		try{
			ObjectMap room = getRelations(Relation::Slave, R_INSIDE);
			for(auto& item: room) {
				item.second->getActions(list, this);
			}
		}
		catch (const std::out_of_range& e) {
			// Nothing needs to be done
		}
    }
    
    void Alive::getActions(ActionList* list, ObjectPointer callee) {
		LOGS("Alive", Verbose) << "Getting actions on " << this->getId() << "." << LOGF;
		if (this == callee) {
			list->addAction(new CallbackAction("suicide", "commit suicide - If you just dont want to live on this planet anymore.",
				RegexMatcher::matcher("commit( a)? suicide|kill me( now)?"),
				[this] (ActionDescriptor * ad) {
						*ad << "You've killed yaself. Cool yeah?";
						this->changeHp(-99999);
				}));
		}
    }
	
	void Alive::registerProperties(IPropertyStorage& storage) {
		storage.have(currentHp, "alive-currenthp", "Current hitpoints")
			.have(maxHp, "alive-maxhp", "Maximum hitpoints")
			.have(attack, "alive-attack", "Attack value")
			.have(defense, "alive-defense", "Defense value");
		IDescriptable::registerProperties(storage);
	}

    
    string Alive::getDescriptionSentence() {
        return RandomString::get()
				<< "You recognize " + this->getName() + "'s figure." << endr
				<< this->getName() + " is there with you." << endr
				<< "You smell the presence of " + this->getName() + "." << endr
				<< this->getName() + " is nearby." << endr;
    }
    
    string Alive::getGroupDescriptionSentence(vector<ObjectPointer> others) {
        if (others.size() == 0) return "";
        else if (others.size() == 1) return getDescriptionSentence();
        
        string sentence;
        for (int i = 0; i < others.size() - 1; i++) {
            if (i != 0) sentence += ", ";
            sentence += others.at(i).safeCast<Alive>()->getName();
        }
        
        sentence += " and " + others.back().safeCast<Alive>()->getName();
		
        return RandomString::get()
				<< "You recognize " + sentence + "." << endr
				<< sentence + " are there with you." << endr
				<< "You smell the presence of " + sentence + "." << endr
				<< sentence + " are nearby." << endr;
    }

	ObjectPointer Alive::getLocation() {
		return getRelations(Relation::Slave, R_INSIDE).begin()->second;
	}

	int Alive::getAttack() const {
		return attack;
	}

	Alive* Alive::setAttack(int attack, ActionDescriptor* ad) {
		if(ad != 0 && this->attack != attack && this->getId() == ad->getAlive()->getId()) {
			*ad << "Your attack value has changed to " + to_string(attack) + ". ";
		}
		this->attack = attack;
		return this;
	}

	int Alive::getDefense() const {
		return defense;
	}

	Alive* Alive::setDefense(int defense, ActionDescriptor* ad) {
		if(ad != 0 && this->defense != defense && this->getId() == ad->getAlive()->getId()) {
			*ad << "Your defense value has changed to " + to_string(defense) + ". ";
		}
		this->defense = defense;
		return this;
	}

	int Alive::getCurrentHp() const {
		return currentHp;
	}

	Alive* Alive::setCurrentHp(int hp, ActionDescriptor* ad) {
		if(ad != 0 && this->currentHp != currentHp && this->getId() == ad->getAlive()->getId()) {
			*ad << "Your current hitpoints have changed to " + to_string(currentHp) + ". ";
		}
		this->currentHp = hp;
		return this;
	}

	int Alive::getMaxHp() const {
		return maxHp;
	}

	Alive* Alive::setMaxHp(int hp, ActionDescriptor* ad) {
		if(ad != 0 && this->maxHp != maxHp && this->getId() == ad->getAlive()->getId()) {
			*ad << "Your maximum hitpoints have changed to " + to_string(maxHp) + ". ";
		}
		this->maxHp = hp;
		return this;
	}

	Alive* Alive::calculateBonuses() {
		// Base Values
		int attack = 1; 
		int defense = 1;
		
		for(int slot = Wearable::Slot::BodyArmor; slot != Wearable::Slot::Invalid; slot--) {
			try {
				ObjectMap worn = this->getRelations(Relation::Master, Wearable::SlotRelations[slot]);
				if(worn.size() > 0) {
					worn.begin()->second.assertType<Wearable>("You've somehow managed to equip non-item. CG. Now contact the administrator to sort it out for you.");
					Wearable* wornItem = worn.begin()->second.unsafeCast<Wearable>();
					attack += wornItem->getAttackBonus();
					defense += wornItem->getDefenseBonus();
				}
			}
			catch (const std::out_of_range& e) {
				
			}
		}
		
		this->setAttack(attack);
		this->setDefense(defense);
		return this;
	}

	Alive* Alive::damageAlive(Alive* attacker, int amount, ActionDescriptor* ad) {
		double multiplier = 10;
		if(getDefense() != 0) { // Division by zero
			multiplier = (double) amount / getDefense();
			if(multiplier > 10) multiplier = 10;
		}
		// Random part
		rand_init(gen, dist, 90, 110);
        int rnd = rand_next(gen, dist);
		
		int damage = (int) (multiplier * rnd / 100 *(amount - getDefense() * 0.33));
		if(ad != 0) {
			if(this->getId() == ad->getAlive()->getId()) {
				*ad << "You have received " + to_string(damage) + " damage from " + attacker->getName() + ". ";
			}
			else {
				*ad << "You have dealt " + to_string(damage) + " to " + this->getName() + ". ";
			}
		}
		changeHp(-damage);
		return this;
	}

	Alive* Alive::changeHp(int amount, ActionDescriptor* ad) {
		this->currentHp += amount;
		if(this->currentHp > this->maxHp) {
			this->currentHp = this->maxHp;
		}
		if(this->currentHp <= 0) {
			this->currentHp = 0;	// Just to be sure
			die();
		}
		return this;
	}

	Alive* Alive::die() {
		// TODO: Oh no, I'm dead, I should respawn
		return this;
	}



	PERSISTENT_IMPLEMENTATION(Alive)
}

