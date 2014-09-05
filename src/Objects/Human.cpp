#include "Human.hpp"
#include "../ActionList.hpp"
#include "../ActionDescriptor.hpp"
#include "Room.hpp"
#include "Inventory.hpp"

namespace Dungeon {
	Human::Human() {
	}

	Human::Human(const Human& orig) {
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
	
	void Human::registerProperties(IPropertyStorage& storage) {
		storage.have(username, "human-username", "Username, public available")
			.have(contact, "human-jid", "Contact JID", false);
		Alive::registerProperties(storage);
	}
	void Human::getActions(ActionList* list, ObjectPointer callee) {
		if (this == callee) {
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
						try {
							ObjectMap backpacks = this->getRelations(Relation::Master, R_INVENTORY);
							for(auto& item : backpacks) {
								if(!item.second->instanceOf(Inventory)) continue;
								*ad << item.second.unsafeCast<Inventory>()->getDescriptionSentence();
							}
						}
						catch (const std::out_of_range& e) {
							
						}
				}));
            
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
				
			// TODO - redo to a MTA. add equiped relations and other inventories
			list->addAction(new CallbackAction("what i own", "what i own - A list of items in backpack",
				RegexMatcher::matcher("what i (have|own)"),
				[this] (ActionDescriptor* ad) {
					bool found = false;
					try {
						ObjectMap backpacks = this->getRelations(Relation::Master, Wearable::SlotRelations[Wearable::Backpack]);
						if(backpacks.size() > 0) {
							backpacks.begin()->second.assertType<Inventory>("You've somehow managed to use not-a-backpack as a backpack. This can't be tolerated.");
							Inventory* backpack = backpacks.begin()->second.unsafeCast<Inventory>();
							*ad << backpack->getContainingSentence();
							found = true;
						}
					}
					catch (const std::out_of_range& e) {
						
					}
					if(!found) {
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
