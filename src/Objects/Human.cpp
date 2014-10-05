#include "Human.hpp"
#include "../ActionList.hpp"
#include "../ActionDescriptor.hpp"
#include "Location.hpp"
#include "Inventory.hpp"
#include "Spell.hpp"
#include <time.h>
#include <cmath>
#include "../exceptions.hpp"
#include "../RandomString.hpp"
#include "../FuzzyStringMatcher.hpp"
#include "../Traps/HardcoreRespawn.hpp"

namespace Dungeon {
	Human::Human() {
		this->setRespawnInterval(DEFAULT_RESPAWN_INTERVAL);	
		this->stats = new int[Stats::End];
		for(int i = 0; i<Stats::End; i++) this->stats[i] = 10;
	}
	
	Human::Human(objId id) : Alive(id) {
		this->setRespawnInterval(DEFAULT_RESPAWN_INTERVAL);	
		this->stats = new int[Stats::End];
		for(int i = 0; i<Stats::End; i++) this->stats[i] = 10;
	}

	Human::Human(objId id, string username, string contact)  :
        Alive(id), username(username), contact(contact) {
		this->setRespawnInterval(DEFAULT_RESPAWN_INTERVAL);
		this->stats = new int[Stats::End];
		for(int i = 0; i<Stats::End; i++) this->stats[i] = 10;
                
                // Creation is the first interaction
                this->markInteraction();
	}


	Human::~Human() {
		delete[] stats;
	}
	
	string Human::getLongName() const {
		return "user " + getUsername() + ", " + getContact();
	}

	string Human::getName() const {
		return getUsername();
	}

    Human* Human::setContact(string contact) {
    	this->contact = contact;
		return this;
    }

    string Human::getContact() const {
    	return contact;
    }

    Human* Human::setUsername(string username) {
    	this->username = username;
		return this;
    }

    string Human::getUsername() const {
    	return username;
	}

	Human* Human::addExperience(int exp, ActionDescriptor* ad) {
		this->exp += exp;
		while(getRequiredExp(getCharacterLevel()+1) <= this->exp) {
			doLevelUp(ad);
		}
		return this;
	}

	int Human::getExperience() const {
		return exp;
	}
	
	void Human::doLevelUp(ActionDescriptor* ad) {
		this->level++;
		this->freepoints += LEVEL_STAT_POINTS;
		if(ad) {
			// FIXME offer user to raise his/her stats
			*ad << "You have just advanced to a new level. You have just gained "
				<< LEVEL_STAT_POINTS 
				<< " stat points. Distribute them by typing 'raise ...'."
				<< eos;
		}
	}

	int Human::getCharacterLevel() const {
		return level;
	}

	Human* Human::changeStatValue(Stats stat, int delta, ActionDescriptor* ad) {
		if(delta == 0) return this;
		if(stat == Stats::End) throw GameException("Invalid stat change requested.");
		if(this->stats[stat] + delta <= 0) throw GameException("Stats must be always positive numbers.");
		this->stats[stat] += delta;
		if(ad) {
			if(delta > 0) {
				*ad << "Your " << getStatName(stat) << " has been raised";
				if(delta == 1) {
					*ad << "." << eos;
				}
				else {
					*ad << " by " << delta << " points." << eos;
				}
			}
			else {
				*ad << "Your " << getStatName(stat) << "has been lowered";
				if(delta == -1) {
					*ad << "." << eos;
				}
				else {
					*ad << " by " << (-delta) << " points." << eos;
				}
			}
		}
		return this;
	}

	Human* Human::setStatValue(Stats stat, int value, ActionDescriptor* ad) {
		if(stat == Stats::End) throw GameException("Invalid stat change requested.");
		if(value <= 0) throw GameException("Stats must be always positive numbers.");
		this->stats[stat] = value;
		if(ad) {
			*ad << "Your " << getStatName(stat) << " has been changed to " << value << "." << eos;
		}
		return this;
	}

	int Human::getStatValue(Stats stat) const {
		if(stat == Stats::End) throw GameException("Invalid stat value requested.");
		return this->stats[stat];
	}

	int Human::getFreePoints() const {
		return this->freepoints;
	}

	Human* Human::useStatPoint(Stats stat, ActionDescriptor* ad) {
		if(getFreePoints() <= 0) return this;
		if(stat == Stats::End) return this;
		this->freepoints--;
		changeStatValue(stat, 1, ad);
		return this;
	}

	string Human::getStatName(Stats stat, bool pure) {
		switch(stat) {
			case Stats::Alchemy:
				if(pure) return "alchemy";
				else return "Alchemy";
			case Stats::Crafting:
				if(pure) return "crafting";
				else return "Crafting";
			case Stats::Dexterity:
				if(pure) return "dexterity";
				else return "Dexterity";
			case Stats::Intelligence:
				if(pure) return "intelligence";
				else return "Intelligence";
			case Stats::Strength:
				if(pure) return "strength";
				else return "Strength";
			case Stats::Vitality:
				if(pure) return "vitality";
				else return "Vitality";
			case Stats::Wisdom: 
				if(pure) return "wisdom";
				else return "Wisdom";
			default:
				if(pure) return "invalid";
				else return "Invalid";
		}
	}

	int Human::getRequiredExp(int level) {
		return (int) 42*pow(1.142,level-1) + 42 * (level-1) * (level-1) + 42 * (level-1) - 42;
	}

	Alive* Human::calculateBonuses() {
		int attack = this->getStatValue(Stats::Strength)/10; 
		int defense = this->getStatValue(Stats::Dexterity)/10;
		int maxhp = this->getStatValue(Stats::Vitality)*20+100;
		
		for(int slot = Wearable::Slot::BodyArmor; slot != Wearable::Slot::Invalid; slot--) {
			try {
				ObjectPointer worn = getSingleRelation(Wearable::SlotRelations[slot], Relation::Master, GameStateInvalid::EquippedMoreThanOne);
				if(!!worn) {
					Wearable* wornItem = worn
							.assertType<Wearable>(GameStateInvalid::EquippedNonWearable)
							.unsafeCast<Wearable>();
					attack += wornItem->getAttackBonus();
					defense += wornItem->getDefenseBonus();
					maxhp += wornItem->getHpBonus();
				}
			}
			catch (const std::out_of_range& e) {
				 
			}
		}
		
		if(attack < 1) attack = 1;
		if(defense < 1) defense = 1;
		this->setAttack(attack)
			->setDefense(defense)
			->setMaxHp(maxhp)
			->save();
		return this;
	}
	
	Alive* Human::die(ActionDescriptor* ad) {
		this->setState(State::Dead);
		if(getGameManager()->getGameMode() == GameManager::Hardcore) {
			HardcoreRespawn* trap = new HardcoreRespawn("HardcoreRespawn/trap/" + RANDID);
			attachTrap(trap, "get-all-actions");
			if(ad) {
				*ad << "You have died and your life has ended. We hope you have enjoyed the Dungeon. "
					<< "If you are brave enough, you can always try again. "
					<< "To restart your journey, simply type anything. "
					<< "Otherwise, goodbye for now. " << eos;
			}
			return this;
		}
		this->setRespawnTime(time(0) + getRespawnInterval());
		if(ad) { // Let's tell what happened
			if(ad->getCaller() == this) { // It is me dying
				*ad << "Oh no! You have just died. "
					<< "Your soul has moved to another plane of existence where it's currently regaining strength. "
					<< "You cannot play for " << getRoundedTime(this->getRespawnInterval()) << ". "
					<< "Type 'respawn' to respawn, when the time comes." << eos;
			}
			else {
				// Actually do we want to notify? if not, remove later
				*ad << this->getName() << " has just died." << eos;
			}
		}
		return this;
	}
	
	Alive* Human::respawn(ActionDescriptor* ad) {
		if(getGameManager()->getGameMode() == GameManager::Hardcore) return this;
		getRespawnLocation().assertExists("No respawning room available");
		getGameManager()->moveAlive(this, getRespawnLocation());
		this->setCurrentHp((int) getMaxHp() * 0.75);
		this->setState(State::Living);
		if(ad != 0) {
			*ad << "You have just respawned in " << getRespawnLocation().safeCast<Location>()->getName() << "." << eos;
		}
		return this;
	}

	void Human::registerProperties(IPropertyStorage& storage) {
		storage.have(username, "human-username", "Username, public available")
			.have(contact, "human-jid", "Contact JID", false)
			.have(craftingLvl, "human-craftinglvl", "Human's crafting lvl")
			.have(craftingExp, "human-craftingexp", "Human's crafting exp")
			.have(level, "human-level", "Human's character level")
			.have(exp, "human-exp", "Human's experience points")
			.have(freepoints, "human-free-points", "Human's stat points to distribute");
		for(int i=Stats::Begin; i<Stats::End; i++) {
			storage.have(stats[i], string("human-stats-") + getStatName(i, true), string("Value of stat ") + getStatName(i));
		}
		Alive::registerProperties(storage);
	}
	
	void Human::getActions(ActionList* list, ObjectPointer callee) {
		triggerTraps("get-actions", nullptr);
		IDescriptable::getActions(list, callee);
		if (this == callee) {
			// Actions always available 
            list->addAction(new CallbackAction("hello", "hello - When you wanna be polite to your Dungeon",
               RegexMatcher::matcher("hello|hi|whats up|wazzup|yo|hey"),
               [this] (ActionDescriptor * ad) {
                   *ad << "Hi!" << eos;
               }, false));
            
            list->addAction(new CallbackAction("who am i", "who am i - In case you forget your identity",
				RegexMatcher::matcher("who( )?am( )?i"),
				[this] (ActionDescriptor * ad) {
					*ad << "You are " + ad->getCaller()->getName() + "." << eos;
				}, false));
				
			// Dead related actions, do not add rest if user is dead	
			if(getState() == State::Dead) {
				list->addAction(new CallbackAction("respawn", "respawn - respawns you in the city", 
					RegexMatcher::matcher("respawn"),
					[this] (ActionDescriptor* ad) {
						if(time(0) >= getRespawnTime()) {
							this->respawn(ad);
						}
						else {
							*ad << "You cannot respawn yet. Wait for another " 
								<< getRoundedTime(getRespawnTime() - time(0)) << " and try again." << eos;
						}
				}));
				
				// This one should match everything except the allowed actions, to inform user to wait
				list->addAction(new CallbackAction("info-dead", "Informs user of death", 
					RegexMatcher::matcher("(?!respawn|who( )?am( )?i|hello|hi|whats up|wazzup|yo).*"),
					[this] (ActionDescriptor* ad) {
						*ad << "You cannot do anything when you are dead." << eos;
						if(time(0) >= getRespawnTime()) {
							*ad << "Please type 'respawn' to become alive again." << eos;
						}
						else {
							*ad << "Please wait another " << getRoundedTime(getRespawnTime() - time(0)) << " and then type respawn." << eos;
						}
						
				},false));
			}
			
			list->addAction(new CallbackAction("suicide", "commit suicide - If you just dont want to live on this planet anymore.",
				RegexMatcher::matcher("commit( a)? suicide|kill me( now)?"),
				[this] (ActionDescriptor * ad) {
						this->changeHp(-9999999, ad);
				}));
			
			list->addAction(new CallbackAction("help", "help - Well...",
				RegexMatcher::matcher("help|what can i do|list actions"),
				[this] (ActionDescriptor * ad) {
						*ad << "This is an non-interactive help. You can do following actions:\n";
						ActionList list;
						this->getAllActions(&list);
                                                
						ad->setReplyFormat(ActionDescriptor::ReplyFormat::List);
						for (auto& a : list) {
                            if (!a.second->isVisibleInHelp) continue;
							a.second->explain(ad);
						}
						ad->setReplyFormat(ActionDescriptor::ReplyFormat::Paragraph);
				}, false));

			list->addAction(new CallbackAction("explore", "explore - List items you can see in your current location",
				RegexMatcher::matcher("explore|examine|(tell me )?where (the fuck )?am i|locate me|tell me my location|investigate"),
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
								for(auto& o : objects) {
									*ad << "\t" + o.first + "\n";
								}
								*ad << eos;
							}
						}
				}));
				
			list->addAction(new CallbackAction("fuck", "",
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
				
			// TODO - redo to a MTA. add equiped relations and other inventories
			list->addAction(new CallbackAction("what do i own", "what do i own - A list of items in backpack",
				RegexMatcher::matcher("(what (do )?i (have|own)|inventory|list items)"),
				[this] (ActionDescriptor* ad) {
					bool  empty = true;
					for(int i = Wearable::Slot::BodyArmor; i != Wearable::Slot::Invalid; i--) {
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
						*ad << ( RandomString::get()
								<< "Your inventory is just empty." << endr
								<< "You're poor as a church mouse." << endr
								<< "You don't have anything." << endr ) << eos;
				}, false));
				
			list->addAction(new CallbackAction("combat stats", "Prints all your combat stats",
				RegexMatcher::matcher(".*combat stats|hitpoints"),
				[this] (ActionDescriptor* ad) {
					Human* me = ad->getCaller();
					*ad << "Here is your current combat profile:" << eos;
					ad->setReplyFormat(ActionDescriptor::ReplyFormat::List);
					*ad << "Hitpoints: " + to_string(me->getCurrentHp()) + "/" + to_string(me->getMaxHp()) << eos;
					*ad << "Attack value: " + to_string(me->getAttack()) << eos;
					*ad << "Defense value: " + to_string(me->getDefense()) << eos;
				}, false));
				
			list->addAction(new CallbackAction("character stats", "Prints all your stats",
				RegexMatcher::matcher(".*character stats|.*human stats"),
				[this] (ActionDescriptor* ad) {
					Human* me = ad->getCaller();
					*ad << "Your current level is " << me->getCharacterLevel() << "." << eos;
					int lastExp, nextExp, progress;
					lastExp = getRequiredExp(me->getCharacterLevel());
					nextExp = getRequiredExp(me->getCharacterLevel()+1);
					progress = ((me->getExperience() - lastExp)*100/(nextExp-lastExp));
					*ad << "You have already received " << progress << "% of experience required for next level up." << eos;
					*ad << "Your current stats are: " << eos;
					ad->setReplyFormat(ActionDescriptor::ReplyFormat::List);
					for(int i = Human::Stats::Begin; i<Human::Stats::End; i++) {
						*ad << Human::getStatName(i) << ": " << me->getStatValue(i) << eos;
					}
					if(me->getFreePoints() > 0) {
						ad->setReplyFormat(ActionDescriptor::ReplyFormat::Paragraph);
						*ad << "I also have " << me->getFreePoints() << " free points to distribute." << eos;
					}
				}, false));
				
			list->addAction(new RaiseStatAction);
                                
			list->addAction(new CallbackAction("resource stats", "Prints all your resource stats",
				RegexMatcher::matcher(".*resource stats|.*resources"),
				[this] (ActionDescriptor* ad) {
					Human* me = ad->getCaller();

					*ad << "Here are your current resources:" << eos;
					ad->setReplyFormat(ActionDescriptor::ReplyFormat::List);
					for (int type = (int)Resource::ResourceType::BEGIN; type != (int)Resource::ResourceType::END; type++) {
						*ad << Utils::capitalize(Resource::ResourceName[type], true) << ": ";
						int quantity = me->getResourceQuantity((Resource::ResourceType)type);
						*ad << "[ " << quantity << " ] " << Resource::getResourceAmountWord(quantity) << eos;
					}
				}, false));
            
            list->addAction(new CallbackAction("self-rename", "",
				RegexMatcher::matcher("rename( myself)?"),
				[this] (ActionDescriptor * ad) {
					*ad << "Well then, what do you want your new name to be?" << eos;
					ad->waitForReply([] (ActionDescriptor *ad, string reply) {
						ad->getCaller()->setUsername(reply)
								->save();
						*ad << "OK. You shall now be called " << ad->getCaller()->getName() << "." << eos; // A common mistake
						*ad << "I'm just curious, why did you change your name?" << eos;
					});
					// Just to show how dialog can be longer :)
					ad->waitForReply([] (ActionDescriptor *ad, string reply) {
						*ad << "Interesting. Now, back to the dungeon!" << eos;
					});
					// Notice that both functions are prepared immediately. They 
					// can else be created after the reply in the closure, but 
					// that render long dialogs overindented and unreadable.
					// 
				}, false));
				
			addCastableSpells(list);
		}
	}

	void Human::addCastableSpells(ActionList* list) {
		CastAction* casting = new CastAction;
		try {
			const ObjectMap& spells = getRelations(Relation::Master, "spell");
			for(auto& spell : spells) {
				spell.second.assertExists("You have a relation to non-existing spell.")
						.assertType<Spell>("You have an invalid relation (spell to non-spell).");
				casting->addTarget(spell.second);
			}
			if(casting->getTargets().size() > 0) {
				list->addAction(casting);
			}
			else {
				delete casting;
			}
		}
		catch (std::out_of_range& e) {
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

	string Human::getWeaponName() const {
		Wearable* weapon = getSingleRelation(Wearable::SlotRelations[Wearable::Slot::Weapon]).safeCast<Wearable>();
		if (weapon)
			return weapon->getName();
		return Alive::getWeaponName();
	}

	string Human::getRoundedTime(int seconds) {
		if(seconds < 60) {
			return to_string(seconds) + " seconds";
		}
		else if(seconds < 3600) {
			return to_string(seconds/60) + " minutes";
		}
		else
			return to_string(seconds/3600) + " hours";
	}

	PERSISTENT_IMPLEMENTATION(Human)
			

	void RaiseStatAction::explain(ActionDescriptor* ad) {
		*ad << "raise ... - raise your character stat." << eos;
	}

	bool RaiseStatAction::match(string command, ActionDescriptor* ad) {
		if (RegexMatcher::match("raise .+", command)) {
			selectStat(command.substr(6), ad);
			return true;
		}
		return false;
	}

	void RaiseStatAction::selectStat(string statName, ActionDescriptor* ad) {
		FuzzyStringMatcher<Human::Stats> matcher;
		matcher.add("strength", Human::Strength)
			.add("str", Human::Strength)
			.add("dexterity", Human::Dexterity)
			.add("dex", Human::Dexterity)
			.add("vitality", Human::Vitality)
			.add("vit", Human::Vitality)
			.add("intelligence", Human::Intelligence)
			.add("int", Human::Intelligence)
			.add("wisdom", Human::Wisdom)
			.add("wis", Human::Wisdom)
			.add("crafting", Human::Crafting)
			.add("craft", Human::Crafting)
			.add("alchemy", Human::Alchemy)
			.add("alch", Human::Alchemy);
		try {
			this->selectedStat = matcher.find(statName);
		}
		catch (exception e) {
			
		}
	}

	void RaiseStatAction::commit(ActionDescriptor* ad) {
		if(selectedStat == Human::End) {
			*ad << "There was no such stat found." << eos;
			return;
		}
		Human* h =  ad->getCaller();
		if(h->getFreePoints() == 0) {
			*ad << "You have no free points to distribute." << eos;
			return;
		}
		h->useStatPoint(selectedStat);
	}

	Human::Stats StatReq::getStat() const {
		return stat;
	}
	
	StatReq* StatReq::setStat(Human::Stats stat) {
		this->stat = stat;
		return this;
	}

	int StatReq::getValue() const {
		return value;
	}
	
	StatReq* StatReq::setValue(int value) {
		this->value = value;
		return this;
	}

	void StatReq::registerProperties(IPropertyStorage& storage) {
		storage.have((int&) stat, "statreq-type", "The required stat.")
				.have(value, "statreq-value", "The required stat value.");
	}

	PERSISTENT_IMPLEMENTATION(StatReq)
}
