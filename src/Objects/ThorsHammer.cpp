#include <sstream>
#include "ThorsHammer.hpp"
#include "../Utils/RegexMatcher.hpp"
#include "../Utils/FuzzyStringMatcher.hpp"
#include "../Actions/CallbackAction.hpp"
#include "../Persistence/WorldDumper.hpp"
#include "../Game/ActionDescriptor.hpp"
#include "../Game/ActionList.hpp"
#include "../Game/GameManager.hpp"

namespace Dungeon {

	ThorsHammer::ThorsHammer() : Item("special/thorshammer") {
		// WARNING - Following leaks may be connected to Archiver, as all of them are surrounded by archiver leaks although 
		// all three of them should be done consecutively by the program
		setName("Thor's Hammer"); // FIXME: Leaks!!! - leaves a trace in valgrind (38B)
		setLongName("The most horrifying item in the world."); // FIXME: Leaks!!! - leave a trace in valgrind (63B)
		setDescription("It reflect's so much light that it cannot be explored further."); //FIXME: Leaks!!! - leaves a trace in valgrind (87B)
		setPickable(false);
		setDropable(false);
		setWeight(0);
		setSize(0);
	}

	ThorsHammer::~ThorsHammer() { }

	void ThorsHammer::getActions(ActionList* list, ObjectPointer callee) {
		list->addAction(new CallbackAction("dump", "(TH) dump - If you want to get some info...",
				RegexMatcher::matcher("^dump( relations)?( of)?(.*)$"),
				[this] (ActionDescriptor * ad) {
					smatch matches;
					RegexMatcher::match("^dump (relations )?(of )?(.*)$", ad->in_msg, matches);
							ObjectPointer target;
					if (matches[3] == "") {
						target = ad->getCaller();
					} else {
						if (!ad->getGM()->hasObject(matches[3])) {
							*ad << "404: Object not found :)" << eos;
							return;
						}
						target = ad->getGM()->getObject(matches[3]);
					}
					*ad << "So you want to know something? Relations which " << target.getId() << " master:\n";
					const RelationList& r = target->getRelations(Relation::Master);
					for (auto& type : r) {
						*ad << "=== " << type.first + ":\n";
						for (auto& obj : type.second) {
							if (obj.second->instanceOf(IDescriptable)) {
								IDescriptable* objptr = obj.second.unsafeCast<IDescriptable>();
								*ad << "\t" << objptr->getName() << "  ...  " << obj.first << "\n";
							} else {
								*ad << "\t" << obj.first << "\n";
							}
						}
					}
					*ad << "Slave:\n";
					const RelationList& s = target->getRelations(Relation::Slave);
					for (auto& type : s) {
						*ad << "=== " << type.first + ":\n";
						for (auto& obj : type.second) {
							if (obj.second->instanceOf(IDescriptable)) {
								IDescriptable* objptr = obj.second.unsafeCast<IDescriptable>();
								*ad << "\t" << objptr->getName() << "  ...  " << obj.first << "\n";
							} else {
								*ad << "\t" << obj.first << "\n";
							}
						}
					}

					*ad << eos;
				}));

		list->addAction(new CallbackAction("shutdown", "(TH) server shutdown - Shutdown's the server.",
				RegexMatcher::matcher("server shutdown"),
				[] (ActionDescriptor * ad) {
					LOGS("ThorsHammer", Warning) << "Admin " << ad->getCaller()->getName() << " initiated server shutdown!!!" << LOGF;
					ad->getGM()->shutdown();
					*ad << "OK. I will just finish the queue. Bye!" << eos;
				}));

		list->addAction(new CallbackAction("dot-dump", "(TH) server dump dot - generate game graph in dot format.",
				RegexMatcher::matcher("server dump dot"),
				[] (ActionDescriptor * ad) {
					LOGS("ThorsHammer", Warning) << "Admin " << ad->getCaller()->getName() << " requested dot dump." << LOGF;
					DotDumper dd;
					*ad << "You can find the dump at " << dd.startDump() << "." << eos;

					for (vector<objId>::value_type& id : ad->getGM()->getObjectList()) {
						ObjectPointer obj = ad->getGM()->getObject(id);
								dd.dumpObject(obj);
					}

					dd.endDump();
				}));

		list->addAction(new CallbackAction("bigBang", "(TH) server initialize - Delete & recreate tables.",
				RegexMatcher::matcher("server initialize"),
				[] (ActionDescriptor * ad) {
					LOGS("ThorsHammer", Warning) << "Admin " << ad->getCaller()->getName() << " requested server reinitialization!!!" << LOGF;
					*ad << "Cross your fingers!" << eos;
					ad->getGM()->initWorld(false);
							*ad << "... can you hear me? Yes? It worked!" << eos;
				}));

		list->addAction(new CallbackAction("heal", "(TH) heal yourself - heals you to full hp.",
				RegexMatcher::matcher("heal( myself| me)?"),
				[] (ActionDescriptor * ad) {
					LOG("ThorsHammer") << "Admin " << ad->getCaller()->getName() << " healed himself using Thors' Hammer." << LOGF;
					ad->getCaller()->setCurrentHp(ad->getCaller()->getMaxHp());
					*ad << "You have fully healed yourself." << eos;
				}));

		list->addAction(new CallbackAction("addexp (int)", "(TH) addexp - increases your level.",
				RegexMatcher::matcher("addexp [0-9]+"),
				[] (ActionDescriptor * ad) {
					LOG("ThorsHammer") << "Admin " << ad->getCaller()->getName() << " increased his experience points using Thors' Hammer." << LOGF;
					string sexp = ad->in_msg.substr(7);
					int exp = stoi(sexp);
					if(exp != 0) {
						ad->getCaller()->addExperience(exp);
						*ad << "You have increased your exp by " << exp << " using your hammer." << eos;
					}
				}, false));

		list->addAction(new CallbackAction("teleport", "(TH) teleport to <id> - teleport yourself everywhere.",
				RegexMatcher::matcher("teleport to .+"),
				[] (ActionDescriptor * ad) {
					smatch matches;
					RegexMatcher::match("^teleport to (.*)$", ad->in_msg, matches);
					string name = matches[1].str();
					if(name.find("/") == string::npos) {
						name = "location/room/" + Utils::decapitalize(name);
					}
					if (!ad->getGM()->hasObject(name)) {
						*ad << "404: Location was not found :)" << eos;
						return;
					}

					LOG("ThorsHammer") << "Admin " << ad->getCaller()->getName() << " teleported to " << name << " using Thors' Hammer." << LOGF;
					ad->getGM()->moveAlive(ad->getCaller(), ad->getGM()->getObject(name));
					*ad << "You have teleported to " 
							<< ad->getCaller()->getLocation().safeCast<IDescriptable>()->getName() << eos;
				}));

		PropertyEditor* pe = new PropertyEditor;
		for (auto pair : callee.safeCast<Alive>()->getLocation()->getRelations(Relation::Master, R_INSIDE)) {
			pe->addTarget(pair.second);
		}
		list->addAction(pe);
	}

	bool ThorsHammer::PropertyEditor::match(const string& command, ActionDescriptor* ad) {
		smatch matches;
		if (RegexMatcher::match("^edit (properties (of )?)?(.+)$", command, matches)) {
			GameManager* gm = ad->getGM();
			if (gm->hasObject(matches[3])) {
				setTarget(gm->getObject(matches[3]));
			} else {
				selectBestTarget(matches[3], ad);
			}
			this->ad = ad;
			LOGS("ThorsHammer", Warning) << "Admin " << ad->getCaller()->getName() << " is editing " << getTarget().getId() << LOGF;
			return true;
		}
		return false;
	}

	bool ThorsHammer::PropertyEditor::handleException(GameException& exception, ActionDescriptor* ad) {
		ad->clearDialog();
		return false; // Process the exception regularly
	}

	void ThorsHammer::PropertyEditor::commitOnTarget(ActionDescriptor* ad, ObjectPointer trg) {
		/**
		 * Instance of object registers its properties as references,
		 * and therefore must not be unloaded in between
		 */
		target = trg;
		target.setLock(true);

		target->beforeLoad(*this);
		target->beforeStore(*this);
		target->registerProperties(*this);
		target->afterStore(*this);
		target->afterLoad(*this);

		askForNextOne(ad);
	}

	void ThorsHammer::PropertyEditor::askForNextOne(ActionDescriptor* ad) {
		if (descriptions.empty()) {
			*ad << "Finished editing!" << eos;
			target->save();
			return;
		}
		string descr = descriptions.front();
		descriptions.pop();
		*ad << "Enter value for " << descr << ". ('.' = skip)" << eos;
	}

	IPropertyStorage& ThorsHammer::PropertyEditor::have(string& prop, string id, string desc, bool editable) {
		if (!editable) return *this;
		descriptions.push(desc + "(" + prop + ")");
		ad->waitForReply([&] (ActionDescriptor* ad, string reply) {
			target.assertExists();
			if (reply != ".") {
				prop = reply;
				LOGS("PropertyEditor", Verbose) << "Set property of " << target.getId() << " to " << prop << LOGF;
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
			target.assertExists();
			if (reply != ".") {
				std::istringstream(reply) >> prop;
				*ad << "Set to '" << prop << "'." << eos;
				LOGS("PropertyEditor", Verbose) << "Set property of " << target.getId() << " to " << prop << LOGF;
			}
			askForNextOne(ad);
		});
		return *this;
	}

	IPropertyStorage& ThorsHammer::PropertyEditor::have(long& prop, string id, string desc, bool editable) {
		if (!editable) return *this;
		std::stringstream ss;
		ss << desc << "(" << prop << ")";
		descriptions.push(ss.str());
		ad->waitForReply([&] (ActionDescriptor* ad, string reply) {
			target.assertExists();
			if (reply != ".") {
				std::istringstream(reply) >> prop;
				*ad << "Set to '" << prop << "'." << eos;
				LOGS("PropertyEditor", Verbose) << "Set property of " << target.getId() << " to " << prop << LOGF;
			}
			askForNextOne(ad);
		});
		return *this;
	}

	IPropertyStorage& ThorsHammer::PropertyEditor::have(bool& prop, string id, string desc, bool editable) {
		if (!editable) return *this;
		descriptions.push(desc + "(" + (prop ? "True" : "False") + ")");
		ad->waitForReply([&] (ActionDescriptor* ad, string reply) {
			target.assertExists();
			if (reply != ".") {
				prop = StringMatcher::matchTrueFalse(reply);
				*ad << "Set to " << (prop ? "True" : "False") << "." << eos;
				LOGS("PropertyEditor", Verbose) << "Set property of " << target.getId() << " to " << (prop ? "True" : "False") << LOGF;
			}
			askForNextOne(ad);
		});
		return *this;
	}

	void ThorsHammer::PropertyEditor::explain(ActionDescriptor* ad) {
		*ad << "edit <id> - alter properties of any object" << eos;
	}

	PERSISTENT_IMPLEMENTATION(ThorsHammer)



}
