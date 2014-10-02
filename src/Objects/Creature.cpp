#include "Creature.hpp"
#include "../RandomString.hpp"
#include "../Dropper.hpp"
#include "../SentenceJoiner.hpp"
#include "../Dropper.hpp"
#include "Human.hpp"
#include <time.h>
#include <vector>

namespace Dungeon {

	Creature* Creature::calculateDrops(ActionDescriptor* ad) {
		bool dropped = false;
		try {
			ObjectMap drops = getRelations(Relation::Master, R_DROP);
			if(!drops.empty()) {
				for(auto& dropIt : drops) {
					ObjectPointer dropPtr = dropIt.second
							.assertExists("Creature has a non-existing drop")
							.assertType<Dropper>("Create has an invalid drop");
					Dropper* drop = dropPtr.unsafeCast<Dropper>();
					if(drop->tryDrop(this->getLocation())) {
						dropped = true;
					}
				}
			}
		}
		catch (const std::out_of_range& e) {
			
		}
		
		if(ad != 0) {
			if(dropped) {
				*ad << this->getName() << " has dropped some items on the ground!" << eos;
			}
			else {
				*ad << this->getName() << " has dropped nothing." << eos;
			}
		}
		return this;
	}

	Creature* Creature::attachDrop(ObjectPointer drop) {
		drop.assertType<Dropper>("You can only attach instances of class Dropper");
		getGameManager()->createRelation(this, drop, R_DROP);
		return this;
	}
	
	Creature* Creature::detachDrop(ObjectPointer drop) {
		drop.assertType<Dropper>("You can only detach instances of class Dropper");
		getGameManager()->removeRelation(this, drop, R_DROP);
		return this;
	}

	Alive* Creature::die(ActionDescriptor* ad) {
		this->calculateDrops(ad);
		if(ad->getAlive()->instanceOf(Human)) {
			Human* h = (Human*) ad->getAlive();
			h->addExperience(getExpReward(), ad);
		}
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
			*ad << "You have killed " << getName() << "." << eos;
		}
		save();
		
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

	Creature* Creature::setExpReward(int reward) {
		this->expReward = reward;
		return this;
	}

	int Creature::getExpReward() const {
		return this->expReward;
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
			list->addAction(new KillAction)
					->addTarget(this);
			return;
		}
		
		list->addAction(new CombatAction)
				->addTarget(this);
	}

	string Creature::getDescriptionSentence() {
		return getGroupDescriptionSentence({ this });
	}

	string Creature::getGroupDescriptionSentence(vector<ObjectPointer> others) {
		SentenceJoiner living, dying, dead;
		
		for (auto i = others.begin(); i != others.end() ; i++) {
			Creature* cr = i->assertType<Creature>().unsafeCast<Creature>();
			switch (cr->getState()) {
				case Alive::State::Living:
					living << cr->getName();
					break;
				case Alive::State::Dying:
					dying << cr->getName();
					break;
				case Alive::State::Dead:
					dead << cr->getName();
					break;
				default:
					LOG("Creature") << "Some invalid creature would like to be explored." << LOGF;
			}
		}
		
		return (string) (RandomString::get()
					<<  living.getSentence("", "You see % wandering around. ") << endr
					<< living.getSentence("", "There is %. ", "There is %. ") << endr)
			+ (string) (RandomString::get()
					<< dying.getSentence("", "You see % lying on the ground heavy breathing. ") << endr
					<< dying.getSentence("", "There lies hardly living body of %. ", "There lie hardly living bodies of %. ") << endr)
			+ (string) (RandomString::get()
					<< dead.getSentence("", "% was killed here.", "% were killed here.") << endr
					<< dead.getSentence("", "There lies corpse of %.", "There lie corpses of %.") << endr);
	}
	
	void KillAction::explain(ActionDescriptor* ad) {
		*ad << "Use 'kill ...' to kill an almost dead creature.\n" << eos;
	}
	
	bool KillAction::match(string command, ActionDescriptor* ad) {
		smatch matches;
		if (RegexMatcher::match("(kill|finish) (.+)", command, matches)) {
			selectBestTarget(matches[2], ad);
			return true;
		}
		return false;
	}
        
	void KillAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		target.assertExists("The target doesn't exist");
		target.assertType<Creature>("You can attack only enemy creatures.");
		target.assertRelation(R_INSIDE, ad->getAlive()->getLocation(), Relation::Slave, "The creature is not here.");
		Creature* creature = target.unsafeCast<Creature>();
		if(creature->getState() == Alive::State::Living) {
			*ad << creature->getName() << " is too healthy to be killed." << eos;
		}
		else if(creature->getState() == Alive::State::Dead || creature->getState() == Alive::State::Invalid) {
			*ad << creature->getName() << " is already dead." << eos;
		}
		else {
			creature->die(ad);
		}
	}
	
	void CombatAction::explain(ActionDescriptor* ad) {
		*ad << "Use 'attack ...' to initiate combat with a creature.\n";
	}
	
	bool CombatAction::match(string command, ActionDescriptor* ad) {
		if (RegexMatcher::match("attack .+", command)) {
			selectBestTarget(command.substr(7), ad);
			return true;
		}
		return false;
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
			*ad << creature->getName() << " is hardly breathing. Finish him!" << eos;
			return false;
		} else if(creature->getState() == Alive::State::Dead) {
			*ad << creature->getName() << " is already dead." << eos;
			return false;
		}
		return true;
	}

	CombatAction::CombatMatch CombatAction::matchAnswer(string reply) {
		FuzzyStringMatcher<CombatAction::CombatMatch> matcher;
		matcher.add("attack", CombatAction::CombatMatch::Attack);
		matcher.add("a", CombatAction::CombatMatch::Attack);
		matcher.add("check", CombatAction::CombatMatch::Check);
		matcher.add("c", CombatAction::CombatMatch::Check);
		matcher.add("status", CombatAction::CombatMatch::Check);
		matcher.add("run", CombatAction::CombatMatch::Run);
		matcher.add("r", CombatAction::CombatMatch::Run);
		
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
				*ad << creature->getName() << " looks very vital." << eos;
			}
			else if(creature->getPercentageHp() > 0.50) {
				*ad << creature->getName() << " is still vital, but you can see some scratches there." << eos;
			}
			else if(creature->getPercentageHp() > 0.25) {
				*ad << creature->getName() << " is wounded, but it is still standing still." << eos;
			}
			else {
				*ad << creature->getName() << " is seriously wounded, finish it while you can!" << eos;
			}
			*ad << text;
			ad->waitForReply([this] (ActionDescriptor* ad, string reply) {
				this->combatLoop(ad, reply);
			});
		}
		else if(action == CombatAction::CombatMatch::Run) {
			ad->getAlive()->damageAlive(creaturePtr, creature->getAttack(), ad);
			if(ad->getAlive()->getState() == Alive::State::Living) {
				*ad << "You have managed to run from " << creature->getName() 
					<< ". Be warned though, as any other action than leaving this room would reinitiate attack." << eos;
			}
		}
		else {
			creature->damageAlive(ad->getAlive(), ad->getAlive()->getAttack(), ad);
			if(creature->getState() == Alive::State::Dying) {
				*ad << creature->getName() << " is mortally wounded. Use 'kill ...' to finish it." << eos;
				return;
			}
			ad->getAlive()->damageAlive(creaturePtr, creature->getAttack(), ad);
			if(ad->getAlive()->getState() != Alive::State::Living) return;
			// Should be able to use potion, or so (later)
			*ad << text;
			*ad << "\n" << creature->getName() << ": [" << Utils::progressBar(creature->getCurrentHp(), creature->getMaxHp(), 10) << "]"
				<< "     You: [" << Utils::progressBar(ad->getAlive()->getCurrentHp(), ad->getAlive()->getMaxHp(), 10) << "]" << eos;
			ad->waitForReply([this] (ActionDescriptor* ad, string reply) {
				this->combatLoop(ad, reply);
			});
		}
	}
	
	Creature* Creature::drops(ObjectPointer item, int chance, int min, int max) {
		Dropper* dropper = new Dropper("dropper/" + getId() + "/" + RANDID);
		dropper->setChance(chance)
				->setMin(min)
				->setMax(max);
		getGameManager()->insertObject(dropper);

		dropper->setItem(item);
		this->attachDrop(dropper);
		return this;
	}


	PERSISTENT_IMPLEMENTATION(Creature)
}
