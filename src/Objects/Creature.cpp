#include "Creature.hpp"
#include "../RandomString.hpp"
#include <time.h>
#include <vector>

namespace Dungeon {

	Alive* Creature::die(ActionDescriptor* ad) {
		if(this->getRespawnInterval() == -1) { // Remove
			// FIXME: Workout a way to tell GM, to delete this after this action
			this->setState(State::Invalid);
			return 0;
		}
		else {
			this->setRespawnTime(time(0) + getRespawnInterval());
			this->setState(State::Dead);
		}
		if(ad != 0) {
			*ad << "You have killed " << getName() << ". ";
		}
		
		return this;
	}
	
	Alive* Creature::changeHp(int amount, ActionDescriptor* ad) {
		setCurrentHp(getCurrentHp() + amount);
		if(getCurrentHp() <= 0) {
			setState(Alive::State::Dying);
		}
		save();
		return this;
	}

	Alive* Creature::respawn(ActionDescriptor* ad) {
		this->setCurrentHp(getMaxHp());
		this->setState(State::Living);
		return this;
	}

	void Creature::getActions(ActionList* list, ObjectPointer callee) {
		// FIXME: The object should be removed, not in the world anymore - check method die()
		if(getState() == State::Invalid) return;
		Alive::getActions(list, callee);
		
		// Something like examine should be here - can also be used on dead body (with different explaining)
		if(getState() == State::Dead) {
			if(time(0) >= getRespawnTime()) {
				respawn();
				save();
			}
			else {
				return;
			}
		}
		else if(getState() == State::Dying) {
			KillAction* kill = new KillAction;
			kill->addTarget(this);
			list->addAction(kill);
			return;
		}
		
		CombatAction* combat = new CombatAction;
		combat->addTarget(this);
		list->addAction(combat);
	}

	string Creature::getDescriptionSentence() {
		switch(getState()) {
			case State::Living:
				return RandomString::get()
					<< "You see " + this->getName() + " wandering around. " << endr
					<< "There is " + this->getName() + ". " << endr;
				break;
			case State::Dying:
				return RandomString::get()
					<< "You see " + this->getName() + " lying on the ground heavy breathing. " << endr
					<< "There lies a hardly living body of " + this->getName() + ". " << endr;
				break;
			default:
				// Dead doesn't need to be seen
				return "";
		}
	}

	string Creature::getGroupDescriptionSentence(vector<ObjectPointer> others) {
		if (others.size() == 0) return "";
        else if (others.size() == 1) return getDescriptionSentence();
		// Do you see a better way?
		vector<ObjectPointer> living, dying;
		for (int i = 0; i < others.size() - 1; i++) {
			if(others.at(i).safeCast<Creature>()->getState() == Alive::State::Living) {
				living.push_back(others.at(i));
			}
			else if(others.at(i).safeCast<Creature>()->getState() == Alive::State::Dying) {
				dying.push_back(others.at(i));
			}
        }
		
		string sentenceLiving, sentenceDying, sentenceFinal;
		for (int i = 0; i < living.size() - 1; i++) {
            if (i != 0) sentenceLiving += ", ";
            sentenceLiving += living.at(i).safeCast<Creature>()->getName();
        }
        sentenceLiving += " and " + living.at(living.size() - 1).safeCast<Creature>()->getName();
		for (int i = 0; i < dying.size() - 1; i++) {
            if (i != 0) sentenceDying += ", ";
            sentenceDying += dying.at(i).safeCast<Creature>()->getName();
        }
        sentenceDying += " and " + dying.at(dying.size() - 1).safeCast<Creature>()->getName();
		
		
		sentenceFinal = RandomString::get()
					<< "You see " + sentenceLiving + " wandering around. " << endr
					<< "There are " + sentenceLiving + ". " << endr;
		sentenceFinal += RandomString::get()
					<< "You see " + sentenceDying + " lying on the ground heavy breathing. " << endr
					<< "There lie hardly living bodies of " + sentenceDying + ". " << endr;
		return sentenceFinal;
	}
	
	void KillAction::explain(ActionDescriptor* ad) {
		*ad << "Use 'kill ...' to kill an almost dead creature.\n";
	}
	
	bool KillAction::matchCommand(string command) {
		return RegexMatcher::match("(kill|finish) .+", command);
	}
        
	void KillAction::commit(ActionDescriptor* ad) {	
		if(ad->in_msg.find("kill ") == 0) {
			commitOnBestTarget(ad, ad->in_msg.substr(5));
		}
		else { // finish
			commitOnBestTarget(ad, ad->in_msg.substr(7));
		}
	}
        
	void KillAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		target.assertExists("The target doesn't exist");
		target.assertType<Creature>("You can attack only enemy creatures.");
		target.assertRelation(R_INSIDE, ad->getAlive()->getLocation(), Relation::Slave, "The creature is not here.");
		Creature* creature = target.unsafeCast<Creature>();
		if(creature->getState() == Alive::State::Living) {
			*ad << creature->getName() << " is too healthy to be killed. ";
		}
		else if(creature->getState() == Alive::State::Dead || creature->getState() == Alive::State::Invalid) {
			*ad << creature->getName() << " is already dead. ";
		}
		else {
			creature->die(ad);
			// TODO: Add drops
		}
	}
	
	void CombatAction::explain(ActionDescriptor* ad) {
		*ad << "Use 'attack ...' to initiate combat with a creature.\n";
	}
	
	bool CombatAction::matchCommand(string command) {
		return RegexMatcher::match("attack .+", command);
	}
        
	void CombatAction::commit(ActionDescriptor* ad) {	
		commitOnBestTarget(ad, ad->in_msg.substr(7));
	}
        
	void CombatAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		this->creaturePtr = target;
		this->combatLoop(ad, "attack");
	}

	bool CombatAction::checkValidity(ActionDescriptor* ad) {
		creaturePtr.assertExists("The target doesn't exist");
		creaturePtr.assertType<Creature>("You can attack only enemy creatures.");
		creaturePtr.assertRelation(R_INSIDE, ad->getAlive()->getLocation(), Relation::Slave, "The creature is not here.");
		Creature* creature = creaturePtr.unsafeCast<Creature>();
		if(creature->getState() == Alive::State::Dying) {
			*ad << creature->getName() << " is hardly breathing. Finish him! ";
			return false;
		} else if(creature->getState() == Alive::State::Dead) {
			*ad << creature->getName() << " is already dead. ";
			return false;
		}
		return true;
	}

	CombatAction::CombatMatch CombatAction::matchAnswer(string reply) {
		FuzzyStringMatcher<CombatAction::CombatMatch> matcher;
		matcher.add("attack", CombatAction::CombatMatch::Attack);
		matcher.add("check", CombatAction::CombatMatch::Check);
		matcher.add("status", CombatAction::CombatMatch::Check);
		matcher.add("run", CombatAction::CombatMatch::Run);
		
		return matcher.find(reply);
	}


	void CombatAction::combatLoop(ActionDescriptor* ad, string reply) {
		if(!checkValidity(ad)) {
			return;
		}
		
		Creature* creature = creaturePtr.unsafeCast<Creature>();
		CombatAction::CombatMatch action = matchAnswer(reply);
		if(action == CombatAction::CombatMatch::Check) {
			if(creature->getPercentageHp() > 0.75) {
				*ad << creature->getName() << " looks very vital. ";
			}
			else if(creature->getPercentageHp() > 0.50) {
				*ad << creature->getName() << " is still vital, but you can see some scratches there. ";
			}
			else if(creature->getPercentageHp() > 0.25) {
				*ad << creature->getName() << " is wounded, but it is still standing still. ";
			}
			else {
				*ad << creature->getName() << " is seriously wounded, finish it while you can! ";
			}
			*ad << "\nType 'attack' to attack the enemy, 'check' to check its status or 'run' to run from the fight.";
			ad->waitForReply([this] (ActionDescriptor* ad, string reply) {
				this->combatLoop(ad, reply);
			});
		}
		else if(action == CombatAction::CombatMatch::Run) {
			ad->getAlive()->damageAlive(creaturePtr, creature->getAttack(), ad);
			if(ad->getAlive()->getState() == Alive::State::Living) {
				*ad << "You have managed to run from " << creature->getName() 
					<< ". Be warned though, as any other action than leaving this room would reinitiate attack. ";
				// FIXME: set a trap to reinitiate attack
			}
		}
		else {
			creature->damageAlive(ad->getAlive(), ad->getAlive()->getAttack(), ad);
			if(creature->getState() == Alive::State::Dying) {
				*ad << creature->getName() << " is mortally wounded. Use 'kill ...' to finish it. ";
				return;
			}
			ad->getAlive()->damageAlive(creaturePtr, creature->getAttack(), ad);
			*ad << "\nType 'attack' to attack the enemy, 'check' to check its status or 'run' to run from the fight.";
			ad->waitForReply([this] (ActionDescriptor* ad, string reply) {
				this->combatLoop(ad, reply);
			});
		}
	}



	
	
	PERSISTENT_IMPLEMENTATION(Creature)
}