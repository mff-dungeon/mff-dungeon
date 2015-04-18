#include "Human.hpp"
#include "Virtual/Base.hpp"
#include "Virtual/Stats.hpp"
#include "../Utils/RandomString.hpp"
#include "../Utils/Config.hpp"
#include "../Game/ActionList.hpp"
#include "../Game/ActionDescriptor.hpp"
#include "../Game/GameManager.hpp"
#include "../exceptions.hpp"
#include "Traps/HardcoreRespawn.hpp"
#include "Location.hpp"
#include "Inventory.hpp"
#include "Spells/Spell.hpp"
#include "Door.hpp"
#include <time.h>
#include <cmath>

namespace Dungeon {

	Human::Human() : Alive() {
		this->setRespawnInterval(Config::RespawnTime());
	}

	Human::Human(const objId& id) : Alive(id) {
		this->setRespawnInterval(Config::RespawnTime());
	}

	Human::Human(const objId& id, const string& username, const string& contact) :
	Alive(id), username(username), contact(contact) {
		this->setRespawnInterval(Config::RespawnTime());
		for (int i = 0; i < Stats::End; i++) this->stats[i] = 10;

		// Creation is the first interaction
		this->markInteraction();
	}

	string Human::getLongName() const {
		return "user " + getUsername() + ", " + getContact();
	}

	string Human::getName() const {
		return getUsername();
	}

	Human* Human::setContact(const string& contact) {
		this->contact = contact;
		return this;
	}

	const string& Human::getContact() const {
		return contact;
	}

	Human* Human::setUsername(const string& username) {
		this->username = username;
		return this;
	}

	const string& Human::getUsername() const {
		return username;
	}

	Human* Human::addExperience(int exp, ActionDescriptor* ad) {
		LOGS(Debug) << getId() << " has acquired " << exp << " experience pts." << LOGF;
		this->exp += exp;
		while (getRequiredExp(getCharacterLevel() + 1) <= this->exp) {
			doLevelUp(ad);
		}
		return this;
	}

	int Human::getExperience() const {
		return exp;
	}

	void Human::doLevelUp(ActionDescriptor* ad) {
		this->level++;
		this->freepoints += Config::LevelStats();
		LOGS(Debug) << getId() << " has acquired a new level." << LOGF;
		if (ad) {
			*ad << "You have just advanced to a new level. You have just gained "
					<< Config::LevelStats()
					<< " stat points. Distribute them by typing 'raise ...'."
					<< eos;
		}
	}

	int Human::getCharacterLevel() const {
		return level;
	}

	Human* Human::changeStatValue(Stats stat, int delta, ActionDescriptor* ad) {
		if (delta == 0) return this;
		if (stat == Stats::End) throw GameException("Invalid stat change requested.");
		if (this->stats[stat] + delta <= 0) throw GameException("Stats must be always positive numbers.");
		this->stats[stat] += delta;
		calculateBonuses();
		if (ad) {
			if (delta > 0) {
				*ad << "Your " << getStatName(stat) << " has been raised";
				if (delta == 1) {
					*ad << "." << eos;
				} else {
					*ad << " by " << delta << " points." << eos;
				}
			} else {
				*ad << "Your " << getStatName(stat) << "has been lowered";
				if (delta == -1) {
					*ad << "." << eos;
				} else {
					*ad << " by " << (-delta) << " points." << eos;
				}
			}
		}
		return this;
	}

	Human* Human::setStatValue(Stats stat, int value, ActionDescriptor* ad) {
		if (stat == Stats::End) throw GameException("Invalid stat change requested.");
		if (value <= 0) throw GameException("Stats must be always positive numbers.");
		this->stats[stat] = value;
		calculateBonuses();
		if (ad) {
			*ad << "Your " << getStatName(stat) << " has been changed to " << value << "." << eos;
		}
		return this;
	}

	int Human::getStatValue(Stats stat) const {
		if (stat == Stats::End) throw GameException("Invalid stat value requested.");
		return this->stats[stat];
	}

	int Human::getFreePoints() const {
		return this->freepoints;
	}

	Human* Human::useStatPoint(Stats stat, ActionDescriptor* ad) {
		if (getFreePoints() <= 0) return this;
		if (stat == Stats::End) return this;
		this->freepoints--;
		changeStatValue(stat, 1, ad);
		return this;
	}

	string Human::getStatName(Stats stat, bool pure) {
		switch (stat) {
			case Stats::Alchemy:
				if (pure) return "alchemy";
				else return "Alchemy";
			case Stats::Crafting:
				if (pure) return "crafting";
				else return "Crafting";
			case Stats::Dexterity:
				if (pure) return "dexterity";
				else return "Dexterity";
			case Stats::Intelligence:
				if (pure) return "intelligence";
				else return "Intelligence";
			case Stats::Strength:
				if (pure) return "strength";
				else return "Strength";
			case Stats::Vitality:
				if (pure) return "vitality";
				else return "Vitality";
			case Stats::Wisdom:
				if (pure) return "wisdom";
				else return "Wisdom";
			default:
				if (pure) return "invalid";
				else return "Invalid";
		}
	}

	int Human::getRequiredExp(int level) {
		return (int) 42 * pow(1.142, level - 1) + 42 * (level - 1) * (level - 1) + 42 * (level - 1) - 42;
	}

	Alive* Human::calculateBonuses() {
		int attack = this->getStatValue(Stats::Strength) / 10;
		int defense = this->getStatValue(Stats::Dexterity) / 10;
		int maxhp = this->getStatValue(Stats::Vitality)*20 + 100;

		for (int slot = Wearable::Slot::BodyArmor; slot != Wearable::Slot::Invalid; slot--) {
			try {
				ObjectPointer worn = getSingleRelation(Wearable::SlotRelations[slot], Relation::Master, GameStateInvalid::EquippedMoreThanOne);
				if (!!worn) {
					Wearable* wornItem = worn
							.assertType<Wearable>(GameStateInvalid::EquippedNonWearable)
							.unsafeCast<Wearable>();
					attack += wornItem->getAttackBonus();
					defense += wornItem->getDefenseBonus();
					maxhp += wornItem->getHpBonus();
				}
			} catch (const std::out_of_range& e) {

			}
		}

		if (attack < 1) attack = 1;
		if (defense < 1) defense = 1;
		this->setAttack(attack)
				->setDefense(defense)
				->setMaxHp(maxhp)
				->save();
		return this;
	}

	Human::Presence Human::getPresence() {
		long unseenTime = (long) time(0) - this->lastInteraction;

		if (unseenTime < 60 * 10) {
			return Presence::Present;
		} else if (unseenTime < 40 * 10) {
			return Presence::Away;
		} else {
			return Presence::Offline;
		}
	}

	Human* Human::markInteraction() {
		this->lastInteraction = (long) time(0);
		return this;
	}

	Alive* Human::die(ActionDescriptor* ad) {
		LOGS(Verbose) << getId() << " has died. What a poor person." << LOGF;
		this->setState(State::Dead);
		if (getGameManager()->getGameMode() == GameManager::Hardcore) {
			HardcoreRespawn* trap = new HardcoreRespawn("HardcoreRespawn/trap/" + RANDID);
			attachTrap(trap, "get-all-actions");
			if (ad) {
				*ad << "You have died and your life has ended. We hope you have enjoyed the Dungeon. "
						<< "If you are brave enough, you can always try again. "
						<< "To restart your journey, simply type anything. "
						<< "Otherwise, goodbye for now. " << eos;
			}
			return this;
		}
		this->setRespawnTime(time(0) + getRespawnInterval());
		if (ad) { // Let's tell what happened
			if (ad->getCaller() == this) { // It is me dying
				*ad << "Oh no! You have just died. "
						<< "Your soul has moved to another plane of existence where it's currently regaining strength. "
						<< "You cannot play for " << getRoundedTime(this->getRespawnInterval()) << ". "
						<< "Type 'respawn' to respawn, when the time comes." << eos;
			} else {
				// Actually do we want to notify? if not, remove later
				*ad << this->getName() << " has just died." << eos;
			}
		}
		return this;
	}

	Alive* Human::respawn(ActionDescriptor* ad) {
		if (getGameManager()->getGameMode() == GameManager::Hardcore) return this;
		getRespawnLocation().assertExists("No respawning room available");
		getGameManager()->moveAlive(this, getRespawnLocation());
		this->setCurrentHp((int) getMaxHp() * 0.75);
		this->setState(State::Living);
		if (ad != 0) {
			*ad << "You have just respawned in " << getRespawnLocation().safeCast<Location>()->getName() << "." << eos;
		}
		return this;
	}

	void Human::registerProperties(IPropertyStorage& storage) {
		storage.have(username, "human-username", "Username, public available")
				.have(contact, "human-jid", "Contact JID", false)
				.have(level, "human-level", "Human's character level")
				.have(exp, "human-exp", "Human's experience points")
				.have(freepoints, "human-free-points", "Human's stat points to distribute")
				.have(lastInteraction, "alive-last-interaction", "UNIX timestamp of last interaction")
				.have(gobackBase, "go-back-base", "The base of the cyclic go-back stack")
				.have(gobackCurrent, "go-back-current", "The current position of the cyclic go-back stack");
		for (int i = Stats::Begin; i < Stats::End; i++) {
			storage.have(stats[i], string("human-stats-") + getStatName(i, true), string("Value of stat ") + getStatName(i));
		}
		Alive::registerProperties(storage);
	}

	void Human::getActions(ActionDescriptor* ad) {
		if (this == ad->getCaller()) {
			auto& list = ad->getActionList();
			
			// Actions always available 
			list.addAction(new CallbackAction("hello", "hello - When you wanna be polite to your Dungeon",
					RegexMatcher::matcher("(hello|hi|whats up|wazzup|yo|hey)( dungeon)?"),
					[this] (ActionDescriptor * ad) {
						*ad << "Hi!" << eos;
					}, false));

			list.addAction(new CallbackAction("who am i", "who am i - In case you forget your identity",
					RegexMatcher::matcher("who( )?am( )?i"),
					[this] (ActionDescriptor * ad) {
						*ad << "You are " + ad->getCaller()->getName() + "." << eos;
					}, false));

			// Dead related actions, do not add rest if user is dead	
			if (getState() == State::Dead) {
				list.addAction(new CallbackAction("respawn", "respawn - respawns you in the city",
						RegexMatcher::matcher("respawn"),
						[this] (ActionDescriptor * ad) {
							if (time(0) >= getRespawnTime()) {
								this->respawn(ad);
							} else {
								*ad << "You cannot respawn yet. Wait for another "
										<< getRoundedTime(getRespawnTime() - time(0)) << " and try again." << eos;
							}
						}));

				// This one should match everything except the allowed actions, to inform user to wait
				list.addAction(new CallbackAction("info-dead", "Informs user of death",
						RegexMatcher::matcher("(?!respawn|who( )?am( )?i|hello|hi|whats up|wazzup|yo).*"),
						[this] (ActionDescriptor * ad) {
							*ad << "You cannot do anything when you are dead." << eos;
							if (time(0) >= getRespawnTime()) {
								*ad << "Please type 'respawn' to become alive again." << eos;
							} else {
								*ad << "Please wait another " << getRoundedTime(getRespawnTime() - time(0)) << " and then type respawn." << eos;
							}

						}, false));
						
				return;
			}

			list.addAction(new CallbackAction("suicide", "commit suicide - If you just dont want to live on this planet anymore.",
					RegexMatcher::matcher("commit( a)? suicide|kill me( now)?|lose( the game)?"),
					[this] (ActionDescriptor* ad) {
						*ad << "Die!" << eos;
						this->changeHp(-9999999, ad);
					}));
					
			list.addAction(new CallbackAction("go back", "returns to previous location", 
					RegexMatcher::matcher("go back|return"), 
					[this] (ActionDescriptor* ad) {
						ObjectPointer backLocation = popGoBackStack();
						if(!backLocation) {
							*ad << "You cannot return to your previous location." << eos;
						}
						else {
							DoorwalkAction da;
							da.addTarget(backLocation);
							da.setTarget(backLocation);
							da.commit(ad);
							popGoBackStack();
						}
					}, false));

			list.addAction(new CallbackAction("help", "help - Well...",
					RegexMatcher::matcher("help|what can i do|list actions"),
					[this] (ActionDescriptor * ad) {
						*ad << "This is an non-interactive help. You can do following actions:\n";
						// TODO - Is this really correct? Will there always be the actions we want?
						ActionList& list = ad->getActionList();
						ad->setReplyFormat(ActionDescriptor::ReplyFormat::List);
						for (auto& a : list) {
							if (!a.second->isVisibleInHelp) continue;
									a.second->explain(ad);
							}
					}, false));

			list.addAction(new CallbackAction("explore", "explore - List items you can see in your current location",
					RegexMatcher::matcher("explore|examine|ls|(tell me )?where (the fuck )?am i|locate me|tell me my location|investigate"),
					[this] (ActionDescriptor * ad) {
						ObjectMap rooms = getRelations(Relation::Slave, R_INSIDE);
						for (auto& room : rooms) {
							ObjectPointer obj = room.second;
									Location* r = obj.safeCast<Location>();
							if (r) {
								r->examine(ad);
							} else {
								*ad << "Non-room location: " << obj.getId() << "\n";
										ObjectMap objects = obj->getRelations(Relation::Master, R_INSIDE);
								for (auto& o : objects) {
									*ad << "\t" + o.first + "\n";
								}
								*ad << eos;
							}
						}
					}));

			list.addAction(new CallbackAction("fuck", "",
					RegexMatcher::matcher("fuck.*"),
					[this] (ActionDescriptor * ad) {
						*ad << (RandomString::get()
								<< "Well, you know, fuck you too." << endr
								<< "Fuck yourself, sir! " << endr
								<< "Server error. Too much grossness in the message." << endr
								<< "Stop behaving like shit and do something useful." << endr) << eos;
						*ad << "You've been charged 50 hitpoints for server atmosphere reconstruction." << eos;
								ad->getCaller()->changeHp(-50, ad);
					}, false));

			list.addAction(new CallbackAction("what do i own", "what do i own - A list of items in backpack",
					RegexMatcher::matcher("(what (do )?i (have|own)|inventory|list items)"),
					[this] (ActionDescriptor * ad) {
						bool empty = true;
						for (int i = Wearable::Slot::BodyArmor; i != Wearable::Slot::Invalid; i--) {
							ObjectPointer equip = getSingleRelation(Wearable::SlotRelations[i], Relation::Master, GameStateInvalid::EquippedMoreThanOne);
							if (!!equip) {
								empty = false;
										*ad << equip.safeCast<Wearable>()->getEquippedSentence() << eos;
								if (equip->instanceOf(Inventory)) {
									*ad << equip.unsafeCast<Inventory>()->getContainingSentence() << eos;
								}
							}
						}
						if (empty)
								*ad << (RandomString::get()
								<< "Your inventory is just empty." << endr
								<< "You're poor as a church mouse." << endr
								<< "You don't have anything." << endr) << eos;
						}, false));
						
			list.addAction(new CallbackAction("stats help", "stats help - Prints help about player statistics", 
					RegexMatcher::matcher("(stats( help)?|my info).*"),
					[this] (ActionDescriptor* ad) {
						*ad << "Here is a list of commands to get details on yourself:" << eos;
						ad->setReplyFormat(ActionDescriptor::List);
						*ad << "To print combat related information, use 'combat stats'." << eos;
						*ad << "To print characted related information, use 'character stats'." << eos;
						*ad << "To print resource related information, use 'resource stats'." << eos;
						ad->setReplyFormat(ActionDescriptor::Paragraph);
					}, true));

			list.addAction(new CallbackAction("combat stats", "Prints all your combat stats",
					RegexMatcher::matcher(".*combat stats|hitpoints"),
					[this] (ActionDescriptor * ad) {
						Human* me = ad->getCaller();
						*ad << "Here is your current combat profile:" << eos;
						ad->setReplyFormat(ActionDescriptor::ReplyFormat::List);
						*ad << "Hitpoints: " + to_string(me->getCurrentHp()) + "/" + to_string(me->getMaxHp()) << eos;
						*ad << "Attack value: " + to_string(me->getAttack()) << eos;
						*ad << "Defense value: " + to_string(me->getDefense()) << eos;
					}, false));

			list.addAction(new CallbackAction("character stats", "Prints all your stats",
					RegexMatcher::matcher(".*char(acter)? stats|.*human stats"),
					[this] (ActionDescriptor * ad) {
						Human* me = ad->getCaller();
						*ad << "Your current level is " << me->getCharacterLevel() << "." << eos;
						int lastExp, nextExp, progress;
						lastExp = getRequiredExp(me->getCharacterLevel());
						nextExp = getRequiredExp(me->getCharacterLevel() + 1);
						progress = ((me->getExperience() - lastExp)*100 / (nextExp - lastExp));
						*ad << "You have received " << progress << "% of experience required for next level up." << eos;
						*ad << "Your current stats are: " << eos;
						ad->setReplyFormat(ActionDescriptor::ReplyFormat::List);
						for (int i = Human::Stats::Begin; i < Human::Stats::End; i++) {
							*ad << Human::getStatName(i) << ": " << me->getStatValue(i) << eos;
						}
						if (me->getFreePoints() > 0) {
							ad->setReplyFormat(ActionDescriptor::ReplyFormat::Paragraph);
							*ad << eos; // Required for a newline after the last stat info
							*ad << "I also have " << me->getFreePoints() << " free points to distribute." << eos;
						}
					}, false));

			list.addAction(new RaiseStatAction);

			list.addAction(new CallbackAction("resource stats", "Prints all your resource stats",
					RegexMatcher::matcher(".*res(ource)? stats|.*resources"),
					[this] (ActionDescriptor * ad) {
						Inventory* pack = ad->getCaller()->getBackpack().unsafeCast<Inventory>();

						*ad << "Here are your current resources:" << eos;
								ad->setReplyFormat(ActionDescriptor::ReplyFormat::List);
						for (int type = (int) Resource::ResourceType::BEGIN; type != (int) Resource::ResourceType::END; type++) {
							*ad << Utils::capitalize(Resource::ResourceName[type], true) << ": ";
							int quantity = pack->getResourceQuantity((Resource::ResourceType)type);
							*ad << "[ " << quantity << " ] " << Resource::getResourceAmountWord(quantity) << eos;
						}
					}, false));

			list.addAction(new CallbackAction("self-rename", "",
					RegexMatcher::matcher("rename( myself)?"),
					[this] (ActionDescriptor * ad) {
						*ad << "Well then, what do you want your new name to be?" << eos;
						ad->waitForReply([] (ActionDescriptor *ad, string reply) {
							ad->getCaller()->setUsername(reply)->save();
							*ad << "OK. You shall now be called " << ad->getCaller()->getName() << "." << eos; // A common mistake
									*ad << "I'm just curious, why did you change your name?" << eos;
						});
						ad->waitForReply([] (ActionDescriptor *ad, string reply) {
							*ad << "Interesting. Now, back to the dungeon!" << eos;
						});
					}, false));

			addCastableSpells(ad);
		}
	}

	void Human::addCastableSpells(ActionDescriptor* ad) {
		CastAction* casting = new CastAction;
		try {
			const ObjectMap& spells = getRelations(Relation::Master, "spell");
			for (auto& spell : spells) {
				spell.second.assertExists("You have a relation to non-existing spell.")
						.assertType<Spell>("You have an invalid relation (spell to non-spell).");
				casting->addTarget(spell.second);
			}
			if (casting->getTargets().size() > 0) {
				ad->getActionList().addAction(casting);
			} else {
				delete casting;
			}
		} catch (std::out_of_range& e) {
			delete casting;
		}
	}

	Human* Human::learnSpell(ObjectPointer spell) {
		spell.assertExists("Trying to learn nothing, huh?")
				.assertType<Spell>("You want to cast what?");
		getGameManager()->createRelation(this, spell, "spell");
		return this;
	}

	bool Human::knowsSpell(ObjectPointer spell) {
		return hasRelation("spell", spell, Relation::Master);
	}
	
	ObjectPointer Human::popGoBackStack() {
		if(gobackBase == gobackCurrent) return ObjectPointer();		
		gobackCurrent--;
		if(gobackCurrent == -1) gobackCurrent = Config::GobackStackSize() - 1;
		string rel = "go-back-stack-" + to_string(gobackCurrent);
		ObjectPointer target = getSingleRelation(rel, Relation::Master);
		if(!!target) {
			bool found = false;
			try {
				for(const auto& door : getLocation()->getRelations(Relation::Master, R_INSIDE)) {
					if(door.second->instanceOf(Door)) {
						for(const auto& loc : door.second->getRelations(Relation::Master, R_TARGET)) {
							if(loc.second == target) {
								target = door.second;
								found = true;
								break;
							}
						}
					}
					if(found) break;
				}
			}
			catch (out_of_range& e) {}
			if(!found) {
				target = ObjectPointer();
				gobackCurrent = gobackBase;
			}
		}
		return target;
	}
	void Human::pushGoBackStack(ObjectPointer op) {
		string rel = "go-back-stack-" + to_string(gobackCurrent);
		setSingleRelation(rel, op);
		gobackCurrent = (gobackCurrent + 1) % Config::GobackStackSize();
		if(gobackBase == gobackCurrent) gobackBase = (gobackBase + 1) % Config::GobackStackSize();
	}

	
	string Human::getWeaponName() const {
		Wearable* weapon = getSingleRelation(Wearable::SlotRelations[Wearable::Slot::Weapon]).safeCast<Wearable>();
		if (weapon)
			return weapon->getName();
		return Alive::getWeaponName();
	}

	string Human::getRoundedTime(int seconds) {
		if (seconds < 120) {
			return to_string(seconds) + " seconds";
		} else if (seconds < 7200) {
			return to_string(seconds / 60) + " minutes";
		} else
			return to_string(seconds / 3600) + " hours";
	}
	
	bool Human::haveSeen(const ObjectPointer& object) {
		return hasRelation(R_SEEN, object);
	}
	
	Human* Human::see(ObjectPointer& object) {
		if (!haveSeen(object))
			getGameManager()->createRelation(this, object, R_SEEN);
		return this;
	}


	PERSISTENT_IMPLEMENTATION(Human)
}
