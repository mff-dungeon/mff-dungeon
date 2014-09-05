#include "Human.hpp"
#include "../ActionList.hpp"
#include "../ActionDescriptor.hpp"
#include "Room.hpp"
#include "Inventory.hpp"
#include <time.h>
#include "../exceptions.hpp"

namespace Dungeon {
	Human::Human() {
		this->setRespawnInterval(DEFAULT_RESPAWN_INTERVAL);	
	}
	
	Human::Human(objId id) : Alive(id) {
		this->setRespawnInterval(DEFAULT_RESPAWN_INTERVAL);	
	}

	Human::Human(objId id, string username, string contact)  :
        Alive(id), username(username), contact(contact) {
		this->setRespawnInterval(DEFAULT_RESPAWN_INTERVAL);
	}


	Human::~Human() {
		
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

	Alive* Human::die(ActionDescriptor* ad) {
		this->setState(State::Dead);
		this->setRespawnTime(time(0) + getRespawnInterval());
		if(ad != 0) { // Let's tell what happened
			if(getId() == ad->getAlive()->getId()) { // It is me dying
			// TODO: Write some fancy method to respawn time nicer (probably rounding to highest order is enough)
				*ad << "Oh no! You have just died. "
					<< "Your soul has moved to another plane of existence where it's currently regaining strength. "
					<< "You cannot play for " << this->getRespawnInterval() << " seconds. "
					<< "Type respawn to respawn, when the time comes. ";
			}
			else {
				// Actually do we want to notify? if not, remove later
				*ad << this->getName() << " has just died.";
			}
		}
		return this;
	}
	
	Alive* Human::respawn(ActionDescriptor* ad) {
		if(!this->getGameManager()->hasObject(getRespawnLocation())) {
			throw GameStateInvalid("No respawning room available.");
		}
		ObjectPointer room(getGameManager(), getRespawnLocation());
		getGameManager()->moveAlive(this, room);
		this->setCurrentHp((int) getMaxHp() * 0.75);
		this->setState(State::Living);
		if(ad != 0) {
			*ad << "You have just respawned in " << room.safeCast<Room>()->getName() << ". ";
		}
		return this;
	}
	
	void Human::registerProperties(IPropertyStorage& storage) {
		storage.have(username, "human-username", "Username, public available")
			.have(contact, "human-jid", "Contact JID", false);
		Alive::registerProperties(storage);
	}
	
	void Human::getActions(ActionList* list, ObjectPointer callee) {
		if (this == callee) {
			// Actions always available 
            list->addAction(new CallbackAction("hello", "hello - When you wanna be polite to your Dungeon",
               RegexMatcher::matcher("hello|hi|whats up|wazzup|yo"),
               [this] (ActionDescriptor * ad) {
                   *ad << "Hi!";
               }, false));
            
            list->addAction(new CallbackAction("who am i", "who am i - In case you forget your identity",
				RegexMatcher::matcher("who( )?am( )?i"),
				[this] (ActionDescriptor * ad) {
					*ad << "You are " + ad->getAlive()->getName() + ".";
				}, false));
				
			// Dead related actions, do not add rest if user is dead	
			if(getState() == State::Dead) {
				list->addAction(new CallbackAction("respawn", "respawn user", 
					RegexMatcher::matcher("respawn"),
					[this] (ActionDescriptor* ad) {
						if(time(0) >= getRespawnTime()) {
							this->respawn(ad);
						}
						else {
							*ad << "You cannot respawn yet. Wait for another " 
								<< (getRespawnTime() - time(0)) << " seconds and try again. ";
						}
				}));
				
				// This one should match everything except the allowed actions, to inform user to wait
				list->addAction(new CallbackAction("info-dead", "Informs user of death", 
					RegexMatcher::matcher("(?!respawn|who( )?am( )?i|hello|hi|whats up|wazzup|yo).*"),
					[this] (ActionDescriptor* ad) {
						*ad << "You cannot do anything when you are dead. ";
						if(time(0) >= getRespawnTime()) {
							*ad << "Please type 'respawn' to become alive again. ";
						}
						else {
							*ad << "Please wait another " << (getRespawnTime() - time(0)) << " seconds and then type respawn. ";
						}
						
				},false));
				// Omit the rest
				return;
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
						for (auto& a : list) {
                            if (!a.second->isVisibleInHelp) continue;
							a.second->explain(ad);
							*ad << "\n";
						}
				}, false));

			list->addAction(new CallbackAction("explore", "explore - List items you can see in your current location",
				RegexMatcher::matcher("explore|(tell me )?where (the fuck )?am i|locate me|tell me my location"),
				[this] (ActionDescriptor * ad) {
						ObjectMap rooms = getRelations(Relation::Slave, R_INSIDE);
						for (auto& room : rooms) {
							ObjectPointer obj = room.second;
							Room* r = obj.safeCast<Room>();
							if (r) {
								r->explore(ad);
							} else {
								*ad << "Non-room location: " << obj->getId() << "\n";
								ObjectMap objects = obj->getRelations(Relation::Master, R_INSIDE);
								for(auto& o : objects) {
									*ad << "\t" + o.first + "\n";
								}
							}
						}
				}));
				
			// TODO - redo to a MTA. add equiped relations and other inventories
			list->addAction(new CallbackAction("what i own", "what i own - A list of items in backpack",
				RegexMatcher::matcher("what i (have|own)"),
				[this] (ActionDescriptor* ad) {
					Inventory* backpack = getBackpack().safeCast<Inventory>();
					if (backpack) {
						*ad << backpack->getContainingSentence();
					} else {
						*ad << "Nothing was found. ";
					}
				}, false));
				
			list->addAction(new CallbackAction("combat stats", "Prints all your combat stats",
				RegexMatcher::matcher(".*combat stats.*"),
				[this] (ActionDescriptor* ad) {
					Alive* me = ad->getAlive();
					*ad << "Hitpoints: " + to_string(me->getCurrentHp()) + "/" + to_string(me->getMaxHp()) + "\n";
					*ad << "Attack value: " + to_string(me->getAttack()) + "\n";
					*ad << "Defense value: " + to_string(me->getDefense());
				}, false));
            
            list->addAction(new CallbackAction("self-rename", "",
				RegexMatcher::matcher("rename( myself)?"),
				[this] (ActionDescriptor * ad) {
					*ad << "Well then, what do you want your new name to be?";
					ad->waitForReply([] (ActionDescriptor *ad, string reply) {
						((Human*) ad->getAlive())->setUsername(reply)
								->save();
						*ad << "OK. You shall now be called " << ad->getAlive()->getName() << ". "; // A common mistake
						*ad << "I'm just curious, why did you change your name?";
					});
					// Just to show how dialog can be longer :)
					ad->waitForReply([] (ActionDescriptor *ad, string reply) {
						*ad << "Interesting. Now, back to the dungeon!";
					});
					// Notice that both functions are prepared immediately. They 
					// can else be created after the reply in the closure, but 
					// that render long dialogs overindented and unreadable.
					// 
				}, false));
		}
	}


	PERSISTENT_IMPLEMENTATION(Human)
}
