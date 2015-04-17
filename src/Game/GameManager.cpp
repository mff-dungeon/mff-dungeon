#include "GameManager.hpp"
#include "../Persistence/WorldCreator.hpp"
#include "ActionQueue.hpp"
#include "../Objects/Human.hpp"
#include <stdexcept>

namespace Dungeon {
	typedef std::shared_ptr<Base> ptr_t;

	GameManager::GameManager(bool init) {
		LOG << "Created." << LOGF;
		gameMode = (GameMode) Config::GameMode();

		int dbCode = DatabaseHandler::getInstance().checkDatabase();
		if (dbCode == DatabaseHandler::E_CONNECTION_ERROR) {
			LOGS(Fatal) << "Database isn't working correctly." << LOGF;
			exit(1);
		} else if (init) {
			LOG << "Cleanup initiated." << LOGF;
			initWorld();
		} else if (dbCode == DatabaseHandler::E_TABLE_COUNT_ERROR) {
			LOG << "Wrong table count detected, cleaning up." << LOGF;
			initWorld();
		}

		aqueue = new ActionQueue(this);
	}

	GameManager::GameMode GameManager::getGameMode() const {
		return gameMode;
	}

	void GameManager::initWorld(bool askOnConsole) {
		int err = 0;
		LOGH("Database cleanup");
		LOG << "Dropping all tables." << LOGF;
		err = DatabaseHandler::getInstance().dropDatabase();
		if (err != DatabaseHandler::E_OK) {
			LOGS(Fatal) << "Drop database query failed." << LOGF;
			exit(1);
		}
		LOG << "All tables dropped." << LOGF;
		err = DatabaseHandler::getInstance().initDatabase();
		if (err != DatabaseHandler::E_OK) {
			LOGS(Fatal) << "Create table database query failed." << LOGF;
			exit(1);
		}

		this->objects.clearTree();

		LOG << "Creating and initializing tables." << LOGF;
		DatabaseHandler::getInstance().beginTransaction();
		WorldCreator wc(this);
		wc.bigBang();
		DatabaseHandler::getInstance().endTransaction();

		/*
		 * Finish
		 */
		LOG << "Initialization completed." << LOGF;
		if (askOnConsole) {
			cout << "Continue with startup? [Y/n]" << std::endl;

			string answer;
			getline(cin, answer);
			if (answer == "n") {
				LOG << "Shutting down" << LOGF;
				exit(0);
			}
		}
		LOGH("Database cleanup finished");
	}

	ptr_t GameManager::getObjectInstance(const objId& id) {
		LOGS(Debug) << "Looking up object with id '" << id << "'." << LOGF;
		ptr_t r = objects.find(id);
		if (r == nullptr) {
			r = ptr_t(loadObject(id));
			if (r != nullptr)
				this->objects.insert(r);
			else
				throw ObjectLost("No such object found");
		}
		return r;
	}

	Base* GameManager::loadObject(const objId& id) {
		Base* r = nullptr;
		r = loader.loadObject(id);
		if (r == nullptr) return 0;
		LOGS(Debug) << "Loaded object '" << id << "' with class " << r->className() << "." << LOGF;
		r->setGameManager(this);

		// Load the relations
		Relation ref_master(id, "0", "0");
		vector<Relation> list_master;
		DatabaseHandler::getInstance().getRelations(list_master, ref_master);
		for (Relation& n : list_master) {
			r->addRelation(n.relation, getObject(n.sid), Relation::Master);
		}

		Relation ref_slave("0", id, "0");
		vector<Relation> list_slave;
		DatabaseHandler::getInstance().getRelations(list_slave, ref_slave);
		for (Relation& n : list_slave) {
			r->addRelation(n.relation, getObject(n.mid), Relation::Slave);
		}
		return r;
	}

	bool GameManager::hasObject(const objId& id) {
		auto r = this->objects.find(id);
		if (r == 0) {
			r = ptr_t(loadObject(id));
			if (r != 0)
				this->objects.insert(r);
			else
				return false;
		}
		LOGS(Debug) << "Peeking object '" << id << "'." << LOGF;
		return true;
	}

	vector<objId> GameManager::getObjectList() {
		return loader.getObjectList();
	}

	bool GameManager::hasObjectLoaded(const objId& id) {
		return this->objects.find(id) != 0;
	}

	ObjectPointer GameManager::getObject(const objId& id) {
		return ObjectPointer(this, id);
	}

	void GameManager::insertObject(Base* obj) {
		LOGS(Debug) << "Inserting object " << obj->getId() << "." << LOGF;
		objects.insert(ptr_t(obj));
		saveObject(obj);
		obj->setGameManager(this);
	}

	void GameManager::saveObject(Base* obj) {
		LOGS(Debug) << "Saving object " << obj->getId() << "." << LOGF;
		loader.saveObject(obj);
	}

	void GameManager::deleteObject(ObjectPointer obj) {
		string id = obj.getId();
		LOGS(Debug) << "Deleting object " << id << "." << LOGF;

		RelationList copy = obj->getRelations(Relation::Master);
		for (const RelationList::value_type& rel : copy) {
			for (const ObjectMap::value_type& pair : rel.second) {
				removeRelation(obj, pair.second, rel.first);
			}
		}

		RelationList copy2 = obj->getRelations(Relation::Slave);
		for (const RelationList::value_type& rel : copy2) {
			for (const ObjectMap::value_type& pair : rel.second) {
				removeRelation(pair.second, obj, rel.first);
			}
		}

		DatabaseHandler::getInstance().deleteObject(obj->getId());
		objects.remove(id);
		LOGS(Verbose) << "Removed object " << id << " from the world." << LOGF;
	}

	void GameManager::addRelation(const Relation& rel) {
		int err = DatabaseHandler::getInstance().addRelation(rel);
		LOGS(Debug) << "Adding relation " << rel.mid << " --> " << rel.sid << "." << LOGF;
		if (err != DatabaseHandler::E_OK) {
			LOGS(Error) << "Error adding relation to database." << LOGF;
		}
	}

	void GameManager::addRelation(ObjectPointer master, ObjectPointer slave, const string& relation) {
		Relation ref(master.getId(), slave.getId(), relation);
		this->addRelation(ref);
	}

	ActionQueue* GameManager::getQueue() {
		return this->aqueue;
	}

	void GameManager::shutdown() {
		this->aqueue->stop();
		if (getGameMode() == GameMode::Test)
			this->objects.clearTree();
	}

	Alive* GameManager::addNewFigure(Alive *figure) {
		this->insertObject(figure);
		figure->setRespawnLocation(ObjectPointer(this, "location/room/landcastle"));

		createRelation(figure,
				Cloner::shallowClone(getObjectInstance("template/inventory/leatherpack/1")),
				Wearable::SlotRelations[Wearable::Slot::Backpack]);
		createRelation(figure,
				Cloner::shallowClone(getObjectInstance("template/wearable/woodenaxe/1")),
				Wearable::SlotRelations[Wearable::Slot::Weapon]);
		createRelation(getObjectInstance("location/room/landcastle"), figure, R_INSIDE);

		figure->calculateBonuses();
		LOGS(Verbose) << "A new figure for player with id " << figure->getId() << " was created." << LOGF;
		return figure;
	}

	void GameManager::createRelation(ObjectPointer master, ObjectPointer slave, const string& relation) {
		Relation rel(master.getId(), slave.getId(), relation);
		this->addRelation(rel);
		if (master.isLoaded())
			master->addRelation(relation, slave, Relation::Master);
		if (slave.isLoaded())
			slave->addRelation(relation, master, Relation::Slave);
	}

	void GameManager::clearRelationsOfType(ObjectPointer obj, const string& relation, Relation::Dir master) {
		if (master) 
			DatabaseHandler::getInstance().deleteRelation(Relation(obj.getId(), "0", relation));
		else 
			DatabaseHandler::getInstance().deleteRelation(Relation("0", obj.getId(), relation));

		// Need to load it because of the "others"
		for (const RelationList::value_type& rel : obj->getRelations(master)) {
			for (const ObjectMap::value_type& pair : rel.second) {
				obj->eraseRelation(relation, pair.second, master);
			}
		}
	}

	bool GameManager::hasRelation(ObjectPointer master, ObjectPointer slave, const string& relation) {
		if (master->hasRelation(relation, slave, Relation::Master) && slave->hasRelation(relation, master, Relation::Slave))
			return true;
		else {
			Relation refRel(master.getId(), slave.getId(), relation);
			bool found = false;
			DatabaseHandler::getInstance().hasRelation(refRel, found);
			return found;
		}
	}

	void GameManager::removeRelation(ObjectPointer master, ObjectPointer slave, const string& relation) {
		if (!master || !slave) return;
		master->eraseRelation(relation, slave, Relation::Master);
		Relation ref_obj(master.getId(), slave.getId(), relation);
		DatabaseHandler::getInstance().deleteRelation(ref_obj);
	}

	void GameManager::moveAlive(Alive* alive, ObjectPointer room) {
		LOGS(Debug) << "Moving alive to " << room->getId() << "." << LOGF;
		// Deals with going back
		if(alive->instanceOf(Human)) {
			Human* h = (Human*) alive;
 			h->pushGoBackStack(alive->getLocation());
		}
		this->removeRelation(alive->getLocation(), alive, R_INSIDE);
		this->createRelation(room, alive, R_INSIDE);
		if (room.safeCast<Location>()->isRespawnable() && alive->instanceOf(Human)) {
			alive->setRespawnLocation(room);
		}
	}

	void GameManager::roundBegin(ActionDescriptor* ad) {
		DatabaseHandler::getInstance().beginTransaction();
	}

	void GameManager::roundEnd(bool noException) {
		if (noException)
			DatabaseHandler::getInstance().endTransaction();
		else
			DatabaseHandler::getInstance().rollback();
	}

}
