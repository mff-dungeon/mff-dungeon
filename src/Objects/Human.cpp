#include "Human.hpp"
#include "../ActionList.hpp"
#include "../ActionDescriptor.hpp"
#include "Room.hpp"

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
	
	void Human::serialize(Archiver& stream) {
		if (stream.isStoring()) {
			stream << username;
			stream << contact;
		} else {
			stream >> username;
			stream >> contact;
		}
		Alive::serialize(stream);
	}
	void Human::getActions(ActionList* list, IObject* callee) {
		if (callee == this) {
			list->addAction(new CallbackAction("dump", "dump - If you want to get some info...",
				RegexMatcher::matcher("dump( relations)?"),
				[this] (ActionDescriptor * ad) {
						*ad << "So you want to know something? Relations which you master:\n";
						RelationList r = this->getRelations(true);
						for (auto& type : r) {
							*ad << type.first + ":\n";
							for(auto& obj : type.second) {
								*ad << "\t" + obj.first + "\n";
							}
						}
						*ad << "Slave:\n";
						r = this->getRelations(false);
						for (auto& type : r) {
							*ad << type.first + ":\n";
							for(auto& obj : type.second) {
								*ad << "\t" + obj.first + "\n";
							}
						}
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
				RegexMatcher::matcher("explore|where (the fuck )?am i|locate me|tell me where (the fuck )?am i|tell me my location"),
				[this] (ActionDescriptor * ad) {
						ObjectMap rooms = this->getRelations(false).at(R_INSIDE);
						for (auto& room : rooms) {
							IObject* obj = room.second.get();
							Room* r = (Room*) obj;
							if (r) {
								r->explore(ad);
							} else {
								*ad << "Non-room location: " << obj->getId() << "\n";
								ObjectMap objects = obj->getRelations(true).at(R_INSIDE);
								for(auto& o : objects) {
									*ad << "\t" + o.first + "\n";
								}
							}
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
