#include <stdexcept>
#include "GameManager.hpp"
#include "WorldCreator.hpp"
#include "dynamic.hpp"
#include "ActionQueue.hpp"
#include "Objects/Human.hpp"

namespace Dungeon {

	GameManager::GameManager(bool init) {
        LOG("GameManager") << "Created." << LOGF;
		loader = new ObjectLoader();
		
		int dbCode = DatabaseHandler::getInstance().checkDatabase();
		if(dbCode == DatabaseHandler::E_CONNECTION_ERROR) {
			LOGS("DatabaseHandler", Fatal) << "Database isn't working" << LOGF;
			exit(1);
		}
		else if(init) {
			LOG("GameManager") << "Cleanup initiated." << LOGF;
			initWorld();
		}
		else if(dbCode == DatabaseHandler::E_TABLE_COUNT_ERROR) {
			LOG("GameManager") << "Wrong table count detected, cleaning up." << LOGF;
			initWorld();
		}
		
		aqueue = new ActionQueue(this);
	}

	void GameManager::initWorld(bool askOnConsole) {
		int err = 0;
		LOGH("Database cleanup");
		LOG("GameManager") << "Dropping all tables." << LOGF;
		err = DatabaseHandler::getInstance().dropDatabase();
		if(err != DatabaseHandler::E_OK) {
			LOGS("GameManager", Fatal) << "Drop database query failed with error code " << err << LOGF;
			exit(1);
		}
		LOG("GameManager") << "All tables dropped." << LOGF;
		err = DatabaseHandler::getInstance().initDatabase();
		if(err != DatabaseHandler::E_OK) {
			LOGS("GameManager", Fatal) << "Create table database query failed with error code " << err << LOGF;
			exit(1);
		}
                
		LOG("GameManager") << "Cleaning object tree." << LOGF;
        this->objects.clearTree();
		
		LOG("GameManager") << "Creating and initializing tables." << LOGF;
		DatabaseHandler::getInstance().beginTransaction();
		WorldCreator wc(this);
		wc.bigBang();
		DatabaseHandler::getInstance().endTransaction();
		
		/*
		 * Finish
		 */
		LOG("GameManager") << "Initialization completed." << LOGF;
		if (askOnConsole) {
                    cout << "Continue with startup? [Y/n]" << std::endl;
		
                    string answer;
                    getline(cin, answer);
                    if (answer == "n") {
                            LOG("GameManager") << "Shutting down" << LOGF;
                            exit(0);
                    }
                }
		LOGH("Database cleanup finished");
	}
	
	IObject* GameManager::getObjectInstance(objId id) {
		IObject * r;
		r = this->objects.find(id);
		if (r == 0) {
			r = loadObject(id);
			if (r != 0)
				this->objects.insert(r);
			else
				throw new ObjectLost("No such object found");
		}
        LOGS("GameManager", Verbose) << "Looking up object '" << id << "'." << LOGF;
		return r;
	}
	
	IObject* GameManager::loadObject(objId id) {
		IObject* r = 0;
		r = loader->loadObject(id);
		if(r == 0) return 0;
                LOGS("GameManager", Info) << "Loaded object '" << id << "' with class " << r->className() << "." << LOGF;
		r->setGameManager(this);
		
		// Load the relations
		Relation* ref_master = new Relation(id, "0", "0", "0", "0");
		vector<Relation*> list_master;
		DatabaseHandler::getInstance().getRelations(list_master, ref_master);
		for(Relation* n : list_master) {
			r->addRelation(n->relation, getObject(n->sid), Relation::Master);
			delete n;
		}
		delete ref_master;
		
		Relation* ref_slave = new Relation("0", id, "0", "0", "0");
		vector<Relation*> list_slave;
		DatabaseHandler::getInstance().getRelations(list_slave, ref_slave);
		for(Relation* n : list_slave) {
			r->addRelation(n->relation, getObject(n->pid), Relation::Slave);
			delete n;
		}
		delete ref_slave;
		return r;
	}
    
    bool GameManager::hasObject(objId id) {
		IObject * r;
		r = this->objects.find(id);
		if (r == 0) {
			r = loadObject(id);
			if (r != 0)
				this->objects.insert(r);
			else
				return false;
		}
        LOGS("GameManager", Verbose) << "Peeking object '" << id << "'." << LOGF;
		return true;
	}
	
	vector<objId> GameManager::getObjectList() {
		return loader->getObjectList();
	}

    
    bool GameManager::hasObjectLoaded(objId id) {
		return this->objects.find(id) != 0;
	}

	ObjectPointer GameManager::getObject(objId id) {
		return ObjectPointer(this, id);
	}

	void GameManager::insertObject(IObject* obj) {
        LOGS("GameManager", Verbose) << "Inserting object " << obj->getId() << "." << LOGF;
		objects.insert(obj);
		saveObject(obj);
		obj->setGameManager(this);
	}
	
	void GameManager::saveObject(IObject* obj) {
        LOGS("GameManager", Verbose) << "Saving object " << obj->getId() << "." << LOGF;
		loader->saveObject(obj);
	}
	
	void GameManager::deleteObject(IObject* obj) {
		LOGS("GameManager", Verbose) << "Deleting object " << obj->getId() << "." << LOGF;
		
		RelationList copy = obj->getRelations(Relation::Master);
		for (RelationList::value_type& rel : copy) {
			for (ObjectMap::value_type& pair : rel.second) {
				removeRelation(obj, pair.second, rel.first);
			}
		}
		
		copy = obj->getRelations(Relation::Slave);
		for (RelationList::value_type& rel : copy) {
			for (ObjectMap::value_type& pair : rel.second) {
				removeRelation(pair.second, obj, rel.first);
			}
		}
		
		DatabaseHandler::getInstance().deleteObject(obj->getId());
		this->objects.remove(obj->getId());
		delete obj;
	}

	void GameManager::addRelation(Relation* rel) {
		int err = DatabaseHandler::getInstance().addRelation(rel);
        LOGS("GameManager", Verbose) << "Adding relation " << rel->pid << " --> " << rel->sid << "." << LOGF;
        if(err != DatabaseHandler::E_OK) {
			LOGS("GameManager", Fatal) << "Error adding relation to database, error code " << err << LOGF;
		}
	}

	void GameManager::addRelation(ObjectPointer master, ObjectPointer slave, string relation) {
		Relation* ref = new Relation(master.getId(), slave.getId(), master->className(), slave->className(), relation);
		this->addRelation(ref);
		delete ref;
	}
	
	ActionQueue* GameManager::getQueue() {
		return this->aqueue;
	}
	
	void GameManager::shutdown() {
		this->aqueue->stop();
	}
    
    Alive* GameManager::addNewFigure(Alive *figure) {
        // TODO: put the figure somewhere, initialize the inventory, and so on...
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
		return figure;
    }
	
	void GameManager::createRelation(ObjectPointer master, ObjectPointer slave, string relation) {
		Relation rel (master.getId(), slave.getId(), master->className(), slave->className(), relation);
		this->addRelation(&rel);
		if (master.isLoaded())
			master->addRelation(relation, slave, Relation::Master);
		if (slave.isLoaded())
			slave->addRelation(relation, master, Relation::Slave);
	}
	
	void GameManager::clearRelationsOfType(ObjectPointer obj, string relation, Relation::Dir master) {
		Relation* ref_obj;
		if (master)
			ref_obj = new Relation(obj.getId(), "0", "0", "0", relation);
		else ref_obj = new Relation("0", obj.getId(), "0", "0", relation);
		DatabaseHandler::getInstance().deleteRelation(ref_obj);
		delete ref_obj;
		
		// Need to load it because of the "others"
		for (const RelationList::value_type& rel : obj->getRelations(master)) {
			for (const ObjectMap::value_type& pair : rel.second) {
				obj->eraseRelation(relation, pair.second, master);
			}
		}
	}
	
	void GameManager::removeRelation(ObjectPointer master, ObjectPointer slave, string relation) {
		if (!master || !slave) return; 
 		master->eraseRelation(relation, slave, Relation::Master);
		Relation* ref_obj = new Relation(master.getId(), slave.getId(), "0", "0", relation);
		DatabaseHandler::getInstance().deleteRelation(ref_obj);
		delete ref_obj;
	}
	
	void GameManager::moveAlive(Alive* alive, ObjectPointer room) {
		this->removeRelation(alive->getLocation(), alive, R_INSIDE);
		this->createRelation(room, alive, R_INSIDE);
		if(room.safeCast<Location>()->isRespawnable() && alive->isInstanceOf(Human::HumanClassName)) {
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
