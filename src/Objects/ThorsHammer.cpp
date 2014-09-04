#include <sstream>
#include "ThorsHammer.hpp"
#include "../GameManager.hpp"
#include "../Actions/CallbackAction.hpp"
#include "../ActionDescriptor.hpp"
#include "../ActionList.hpp"
#include "../WorldDumper.hpp"

namespace Dungeon
{
	ThorsHammer::ThorsHammer() : Item("special/thorshammer") {
		setName("Thor's Hammer");
		setLongName("The most horrifying item in the world.");
		setDescription("It reflect's so much light that it cannot be explored further.");
		setPickable(false);
		setDropable(false);
		setWeight(0);
		setSize(0);
	}
	
	ThorsHammer::~ThorsHammer() {
	}

	void ThorsHammer::getActions(ActionList* list, ObjectPointer callee) {
		list->addAction(new CallbackAction("dump", "dump - If you want to get some info...",
		RegexMatcher::matcher("^dump( relations)?( of)?(.*)$"),
		[this] (ActionDescriptor * ad) {
				smatch matches;
				RegexMatcher::match("^dump (relations )?(of )?(.*)$", ad->in_msg, matches);
				ObjectPointer target;
				if (matches[3] == "") {
					target = ad->getAlive();
				} else {
					if (!ad->getGM()->hasObject(matches[3])) {
						*ad << "404: Object not found :)";
						return;
					}
					target = ad->getGM()->getObjectPointer(matches[3]);
				}
				*ad << "So you want to know something? Relations which " << target->getId() << " master:\n";
				RelationList r = target->getRelations(Relation::Master);
				for (auto& type : r) {
					*ad << "=== " << type.first + ":\n";
					for(auto& obj : type.second) {
						if(obj.second->instanceOf(IDescriptable)) {
							IDescriptable* objptr = obj.second.unsafeCast<IDescriptable>();
							*ad << "\t" << objptr->getName() << "  ...  " << obj.first << "\n";
						}
						else {
							*ad << "\t" << obj.first << "\n";
						}
					}
				}
				*ad << "Slave:\n";
				r = target->getRelations(Relation::Slave);
				for (auto& type : r) {
					*ad << "=== " << type.first + ":\n";
					for(auto& obj : type.second) {
						if(obj.second->instanceOf(IDescriptable)) {
							IDescriptable* objptr = obj.second.unsafeCast<IDescriptable>();
							*ad << "\t" << objptr->getName() << "  ...  " << obj.first << "\n";
						}
						else {
							*ad << "\t" << obj.first << "\n";
						}
					}
				}
		}));
		
		list->addAction(new CallbackAction("shutdown", "server shutdown - Shutdown's the server.",
				RegexMatcher::matcher("server shutdown"), 
				[] (ActionDescriptor* ad) {
					ad->getGM()->shutdown();
					*ad << "OK. I will just finish the queue. Bye!";
				}));
		
		list->addAction(new CallbackAction("dot-dump", "server dump dot - generate game graph in dot format.",
				RegexMatcher::matcher("server dump dot"), 
				[] (ActionDescriptor* ad) {
					DotDumper dd;
					*ad << "You can find the dump at " << dd.startDump() << ".";
					
					for (vector<objId>::value_type& id : ad->getGM()->getObjectList()) {
						ObjectPointer obj = ad->getGM()->getObjectPointer(id);
						dd.dumpObject(obj);
					}

					dd.endDump();
				}));
		
		list->addAction(new CallbackAction("bigBang", "server initialize - Delete & recreate tables.",
				RegexMatcher::matcher("server initialize"), 
				[] (ActionDescriptor* ad) {
					*ad << "Cross your fingers! ";
					ad->getGM()->initWorld(false);
					*ad << "... can you hear me? Yes? It worked!";
				}));
				
		list->addAction(new CallbackAction("heal", "heal yourself - heals you to full hp.",
				RegexMatcher::matcher("heal"),
				[] (ActionDescriptor* ad) {
					ad->getAlive()->setCurrentHp(ad->getAlive()->getMaxHp());
					*ad << "You have fully healed yourself. ";
				}));
		
		list->addAction(new CallbackAction("teleport", "teleport <id> - teleport yourself everywhere.",
				RegexMatcher::matcher("teleport .+"), 
				[] (ActionDescriptor* ad) {
					smatch matches;
					RegexMatcher::match("^teleport (.*)$", ad->in_msg, matches);
					if (!ad->getGM()->hasObject(matches[3])) {
						*ad << "404: Object not found :)";
						return;
					}
					
					ad->getGM()->moveAlive(ad->getAlive(), matches[3]);
				}));
				
		list->addAction(new PropertyEditor);
	}
	
	bool ThorsHammer::PropertyEditor::matchCommand(string command) {
		return RegexMatcher::match("edit .+", command);
	}
	
	void ThorsHammer::PropertyEditor::commit(ActionDescriptor* ad) {
		smatch matches;
		RegexMatcher::match("^edit (properties (of )?)?(.+)$", ad->in_msg, matches);
		
		LOG("TH") << "Editing " << matches[3] << LOGF;
		GameManager* gm = ad->getGM();
		if (!gm->hasObject(matches[3])) {
			*ad << "404: Object not found :)";
			return;
		}
		target = gm->getObjectPointer(matches[3]);
		
		target->beforeLoad(*this);
		target->beforeStore(*this);
		target->registerProperties(*this);
		target->afterStore(*this);
		target->afterLoad(*this);
		
		askForNextOne(ad);
	}
	
	void ThorsHammer::PropertyEditor::askForNextOne(ActionDescriptor* ad) {
		if (descriptions.empty()) {
			*ad << "Finished editing!";
			target->save();
			return;
		}
		string descr = descriptions.front();
		descriptions.pop();
		*ad << "Enter value for " << descr << ". ('.' = skip)";
	}
	
	IPropertyStorage& ThorsHammer::PropertyEditor::have(string& prop, string id, string desc, bool editable) {
		if (!editable) return *this;
		descriptions.push(desc + "(" + prop + ")");
		ad->waitForReply([&] (ActionDescriptor* ad, string reply) {
			if (reply != ".") {
				prop = reply;
				LOG("PropEditor") << "Set property of " << target->getId() << " to " << prop << LOGF;
			}
			askForNextOne(ad);
		});
		return *this;
	}
	
	IPropertyStorage& ThorsHammer::PropertyEditor::have(int& prop, string id, string desc, bool editable) {
		if (!editable) return *this;
		std::stringstream ss;
		ss << desc << "(" << prop << ")";
		descriptions.push(ss.str());
		ad->waitForReply([&] (ActionDescriptor* ad, string reply) {
			if (reply != ".") {
				std::istringstream(reply) >> prop;
				*ad << "Set to '" << prop << "'. ";
				LOG("PropEditor") << "Set property of " << target->getId() << " to " << prop << LOGF;
			}
			askForNextOne(ad);
		});
		return *this;
	}
	
	IPropertyStorage& ThorsHammer::PropertyEditor::have(bool& prop, string id, string desc, bool editable) {
		if (!editable) return *this;
		descriptions.push(desc + "(" + (prop ? "True" : "False") + "). 1/true for true, 0/false for false");
		ad->waitForReply([&] (ActionDescriptor* ad, string reply) {
			if (reply != ".") {
				transform(reply.begin(), reply.end(), reply.begin(), ::tolower);
				prop = (reply == "true" || reply == "1");
				*ad << "Set to " << (prop ? "True" : "False") << ". ";
				LOG("PropEditor") << "Set property of " << target->getId() << " to " << (prop ? "True" : "False") << LOGF;
			}
			askForNextOne(ad);
		});
		return *this;
	}
	
	void ThorsHammer::PropertyEditor::explain(ActionDescriptor* ad) {
		*ad << "You can edit all properties of any object with \"edit <id>\".\n";
	}

    PERSISTENT_IMPLEMENTATION(ThorsHammer)
			
	
			
}
