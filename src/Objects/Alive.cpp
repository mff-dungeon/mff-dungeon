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
		
		// Get actions for the inventory items - now searches backpacks + thors hammer
		LOGS("Alive", Verbose) << "Getting actions on inventory - " << this->getId() << "." << LOGF;
		try{
			RelationList mastering = getRelations(Relation::Master);
			for (auto& pair : mastering)
				for(auto& item: pair.second) {
					item.second.get()->getActions(list, this);
				}
		}
		catch (const std::out_of_range& e) {
			// Nothing needs to be done
		}
		
		LOGS("Alive", Verbose) << "Getting actions in location - " << this->getId() << "." << LOGF;
		// Find objects in current location
		try{
			ObjectMap room = getRelations(Relation::Slave, R_INSIDE);
			for(auto& item: room) {
				item.second.get()->getActions(list, this);
			}
		}
		catch (const std::out_of_range& e) {
			// Nothing needs to be done
		}
    }
    
    void Alive::getActions(ActionList* list, IObject *callee) {
		LOGS("Alive", Verbose) << "Getting actions on " << this->getId() << "." << LOGF;
		if (callee == this) {
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
    
    string Alive::getGroupDescriptionSentence(vector<IDescriptable *> others) {
        if (others.size() == 0) return "";
        else if (others.size() == 1) return getDescriptionSentence();
        
        string sentence;
        for (int i = 0; i < others.size() - 1; i++) {
            if (i != 0) sentence += ", ";
            sentence += others.at(i)->getName();
        }
        
        sentence += " and " + others.at(others.size() - 1)->getName();
		
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

	Alive* Alive::setAttack(int attack) {
		this->attack = attack;
		return this;
	}

	int Alive::getDefense() const {
		return defense;
	}

	Alive* Alive::setDefense(int defense) {
		this->defense = defense;
		return this;
	}

	int Alive::getCurrentHp() const {
		return currentHp;
	}

	Alive* Alive::setCurrentHp(int hp) {
		this->currentHp = hp;
		return this;
	}

	int Alive::getMaxHp() const {
		return maxHp;
	}

	Alive* Alive::setMaxHp(int hp) {
		this->maxHp = hp;
		return this;
	}

	Alive* Alive::damageAlive(int amount) {
		double multiplier = 10;
		if(getDefense() != 0) { // Division by zero
			multiplier = (double) amount / getDefense();
			if(multiplier > 10) multiplier = 10;
		}
		int damage = (int) (multiplier * (amount - getDefense() * 0.33));
		changeHp(-damage);
		return this;
	}

	Alive* Alive::changeHp(int amount) {
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

