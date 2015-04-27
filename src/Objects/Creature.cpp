#include "Creature.hpp"
#include "Human.hpp"
#include "../Utils/RandomString.hpp"
#include "../Utils/SentenceJoiner.hpp"
#include "../Utils/RegexMatcher.hpp"
#include "../Game/ActionDescriptor.hpp"
#include "../Game/GameManager.hpp"
#include "../Output/ProgressBar.hpp"
#include "Virtual/Dropper.hpp"
#include "Virtual/Dropper.hpp"
#include <time.h>
#include <vector>
#include <functional>
namespace Dungeon {

	Creature* Creature::calculateDrops(ActionDescriptor* ad) {
		SentenceJoiner dropInfo;
		ObjectGroup droppedItems;
		try {
			const ObjectMap& drops = getRelations(Relation::Master, R_DROP);
			if (!drops.empty()) {
				for (auto& dropIt : drops) {
					ObjectPointer dropPtr = dropIt.second
							.assertExists("Creature has a non-existing drop")
							.assertType<Dropper>("Create has an invalid drop");
					Dropper* drop = dropPtr.unsafeCast<Dropper>();
					ObjectGroup droppedItem = drop->tryDrop(this->getLocation(), dropInfo);
					droppedItems.insert(droppedItem.begin(), droppedItem.end());
				}
			}
		} catch (const std::out_of_range& e) {

		}

		if (ad != nullptr) {
			*ad << dropInfo.getSentence("It has dropped nothing.",
					"It has dropped %.") << eos;
			for (auto& item : droppedItems) {
				ad->getCaller()->see(item.second);
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
		if (ad != nullptr) {
			*ad << "You have killed " << getName() << "." << eos;
		}
		this->calculateDrops(ad);
		ad->getCaller()->addExperience(getExpReward(), ad);
		if (this->getRespawnInterval() == -1) { // Remove
			// FIXME: Workout a way to tell GM, to delete this after this action
			// OH: Perhaps just remove it now? OP will handle it.
			this->setState(State::Invalid);
			return 0;
		} else {
			this->setRespawnTime(time(0) + getRespawnInterval());
			this->setState(State::Dead);
		}
		save();

		return this;
	}

	Alive* Creature::changeHp(int amount, ActionDescriptor* ad) {
		setCurrentHp(getCurrentHp() + amount);
		if (getCurrentHp() <= 0) {
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

	void Creature::getActions(ActionDescriptor* ad) {
		// FIXME: The object should be removed, not in the world anymore - check method die()
		LOGS(Debug) << "Listing all actions on " << getId() << "." << LOGF;
		if (getState() == State::Invalid) {
			LOGS(Warning) << "A creature " << getId() << " is still existing even though it should have been removed from the world." << LOGF;
			return;
		}

		// Something like examine should be here - can also be used on dead body (with different explaining)
		if (getState() == State::Dead) {
			if (time(0) >= getRespawnTime()) {
				respawn();
				save();
			} else {
				return;
			}
		} else if (getState() == State::Dying) {
			ad->getActionList().addAction(new KillAction)
					->addTarget(this);
			return;
		}

		ad->getActionList().addAction(new CombatAction)
				->addTarget(this);
	}

	void Creature::examine(ActionDescriptor* ad) {
		triggerTraps("examine", ad);
		if (ad) {
			*ad << getDescription();
			double percent = (double) ad->getCaller()->calculateDamage(this, getAttack())/ad->getCaller()->getMaxHp();
			if(percent > 0.5) {
				*ad << "This thing is a killer, I should not come any closer!";
			}
			else if(percent > 0.2) {
				*ad << "It's definitely strong, but I could make it with some friends.";
			}
			else if(percent > 0.07) {
				*ad << "I should be careful when attacking this.";
			}
			*ad << eos;
		}
	}

	string Creature::getDescriptionSentence() {
		return getGroupDescriptionSentence({this});
	}

	string Creature::getGroupDescriptionSentence(vector<ObjectPointer> others) {
		SentenceJoiner living, dying, dead;

		for (auto i = others.begin(); i != others.end(); i++) {
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
					LOG << "Some invalid creature would like to be explored." << LOGF;
			}
		}

		return (string) (RandomString::get()
				<< living.getSentence("", "You see % wandering around. ") << endr
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

	bool KillAction::match(const string& command, ActionDescriptor* ad) {
		smatch matches;
		if (RegexMatcher::match("(kill|finish)( (.+))?", command, matches)) {
			selectBestTarget(matches[3], ad);
			return true;
		}
		return false;
	}

	void KillAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		target.assertExists("The target doesn't exist");
		target.assertType<Creature>("You can attack only enemy creatures.");
		target.assertRelation(R_INSIDE, ad->getCaller()->getLocation(), Relation::Slave, "The creature is not here.");
		Creature* creature = target.unsafeCast<Creature>();
		if (creature->getState() == Alive::State::Living) {
			*ad << creature->getName() << " is too healthy to be killed." << eos;
		} else if (creature->getState() == Alive::State::Dead || creature->getState() == Alive::State::Invalid) {
			*ad << creature->getName() << " is already dead." << eos;
		} else {
			creature->die(ad);
		}
	}

	void CombatAction::explain(ActionDescriptor* ad) {
		*ad << "Use 'attack ...' to initiate combat with a creature.\n";
	}

	bool CombatAction::match(const string& command, ActionDescriptor* ad) {
		RegexMatcher::matches matches;
		if (RegexMatcher::match("attack( .+)?", command, matches)) {
			selectBestTarget(matches[1], ad);
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
		creaturePtr.assertRelation(R_INSIDE, ad->getCaller()->getLocation(), Relation::Slave, "The creature is not here.");
		Creature* creature = creaturePtr.unsafeCast<Creature>();
		if (creature->getState() == Alive::State::Dying) {
			*ad << creature->getName() << " is hardly breathing. Finish him!" << eos;
			return false;
		} else if (creature->getState() == Alive::State::Dead) {
			*ad << creature->getName() << " is already dead." << eos;
			return false;
		}
		return true;
	}

	CombatAction::CombatMatch CombatAction::matchAnswer(const string& reply) {
		static FuzzyStringMatcher<CombatMatch> matcher;
		switch (combatState) {
			case CombatMatch::Ask:
				if (matcher.empty()) {
					matcher.add("a", CombatMatch::Attack);
					matcher.add("s", CombatMatch::Attack);
					matcher.add("c", CombatMatch::Check);
					matcher.add("r", CombatMatch::Run);
					matcher.add("h", CombatMatch::Help);
					matcher.add("x", CombatMatch::Berserk);
					matcher.add("berserk", CombatMatch::Berserk);
					matcher.add("fight until death", CombatMatch::Berserk);
					matcher.add("fight till death", CombatMatch::Berserk);
					matcher.add("attack", CombatMatch::Attack);
					matcher.add("strike", CombatMatch::Attack);
					matcher.add("check", CombatMatch::Check);
					matcher.add("status", CombatMatch::Check);
					matcher.add("run", CombatMatch::Run);
					matcher.add("help", CombatMatch::Help);
				}
				try {
					return matcher.find(reply);
				} catch (GameException& ge) {
					return CombatMatch::Help;
				}
			default:
				auto mode = combatState;
				combatState = CombatMatch::Ask;
				return mode;
		}
	}

	void CombatAction::combatLoop(ActionDescriptor* ad, const string& reply) {
		if (!checkValidity(ad)) {
			return;
		}
		CombatMatch action = matchAnswer(reply);
		combatStep(ad, action);
	}

	void CombatAction::combatStep(ActionDescriptor* ad, CombatMatch action) {
		Creature* creature = creaturePtr.unsafeCast<Creature>();
		Human* human = ad->getCaller();
		// TODO: Add some actually useful info :)
		if (action == CombatMatch::Check) {
			if (creature->getPercentageHp() > 0.75) {
				*ad << creature->getName() << " looks very vital." << eos;
			} else if (creature->getPercentageHp() > 0.50) {
				*ad << creature->getName() << " is still vital, but you can see some scratches there." << eos;
			} else if (creature->getPercentageHp() > 0.25) {
				*ad << creature->getName() << " is wounded, but it is still standing still." << eos;
			} else {
				*ad << creature->getName() << " is seriously wounded, finish it while you can!" << eos;
			}
		}

		size_t sumDamage = 0;

		// User strikes
		if (action == CombatMatch::Attack || action == CombatMatch::Berserk) {
			int damage = creature->calculateDamage(human, human->getAttack());
			sumDamage += damage;
			creature->damageAlive(human, damage, ad);
			if (creature->getState() == Alive::State::Dying) {
				creature->die(ad);
				return;
			}
		}

		// Creature strikes
		if (action != CombatMatch::Check) {
			int damage = human->calculateDamage(creaturePtr, creature->getAttack());
			sumDamage += damage;
			human->damageAlive(creaturePtr, damage, ad);
		}
		if (human->getState() != Alive::State::Living) return;

		if (action == CombatMatch::Run) {
			// TODO: Some dexterity-based randomness?
			*ad << "You have managed to run from " << creature->getName() << "." << eos;
			return;
		}

		if (!combatModeInformed || action == CombatAction::Help) {
			ad->flushContainers();
			*ad << "\nYou are in fight, you have to act fast. To strike the enemy, write 's'. To run from the fight, type 'r'. To get this information again, type 'h'." << eos;
			if (sumDamage) *ad << "If you're brave, cross your fingers and fight till death with 'x'." << eos;
			*ad << "Currently, you can't use your special skills and equipment during a fight." << eos;
			combatModeInformed = true;
		}

		ad->flushContainers();
		Output::Container& out = ad->getOutputContainer();
		out.emplace<Output::FormattedString>("b", creature->getName());
		out << ": ";
		out.emplace<Output::ProgressBar>(creature->getCurrentHp(), creature->getMaxHp(), 10, "#FF0000");
		out << "     ";
		out.emplace<Output::FormattedString>("b", human->getName());
		out << ": ";
		out.emplace<Output::ProgressBar>(human->getCurrentHp(), human->getMaxHp(), 10, "#00FF00");

		if (action == CombatMatch::Berserk && sumDamage)
			return combatStep(ad, CombatMatch::Berserk);
		ad->waitForReply(this, &CombatAction::combatLoop);
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
