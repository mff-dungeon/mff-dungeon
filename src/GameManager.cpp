#include <stdexcept>
#include <c++/4.9/stdexcept>

#include "GameManager.hpp"
#include "WorldCreator.hpp"

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

	void GameManager::initWorld() {
		int err = 0;
		LOGH("Database cleanup");
		LOG("GameManager") << "Dropping all tables." << LOGF;
		err = DatabaseHandler::getInstance().dropDatabase();
		if(err != DatabaseHandler::E_OK) {
			LOGS("GameManager", Fatal) << "Drop database query failed with error code " << err << LOGF;
			exit(1);
		}
		LOG("GameManager") << "All tables dropped, creating and initializing tables." << LOGF;
		err = DatabaseHandler::getInstance().initDatabase();
		if(err != DatabaseHandler::E_OK) {
			LOGS("GameManager", Fatal) << "Create table database query failed with error code " << err << LOGF;
			exit(1);
		}
		
		WorldCreator wc(this);
		wc.bigBang();
		
		/*
		 * Finish
		 */
		LOG("GameManager") << "Initialization completed." << LOGF;
		cout << "Continue with startup? [Y/n]" << std::endl;
		
		string answer;
		getline(cin, answer);
		if (answer == "n") {
			LOG("GameManager") << "Shutting down" << LOGF;
			exit(0);
		}
		LOGH("Database cleanup finished");
	}
	
	IObject* GameManager::getObject(objId id) {
		IObject * r;
		r = this->objects.find(id);
		if (r == 0) {
			r = loadObject(id);
			if (r != 0)
				this->objects.insert(r);
			else
				throw new runtime_error("No such object found");
		}
        LOGS("GameManager", Verbose) << "Looking up object '" << id << "'." << LOGF;
		return r;
	}
	
	IObject* GameManager::loadObject(objId id) {
        LOGS("GameManager", Info) << "Loading object '" << id << "'." << LOGF;
		IObject* r = 0;
		r = loader->loadObject(id);
		if(r == 0) return 0;
		// Load the relations
		Relation* ref_master = new Relation(id, "0", "0", "0", "0");
		vector<Relation*> list_master;
		DatabaseHandler::getInstance().getRelations(list_master, ref_master);
		for(Relation* n : list_master) {
			r->addRelation(n->relation, getObjectPointer(n->sid), true);
			delete n;
		}
		delete ref_master;
		
		Relation* ref_slave = new Relation("0", id, "0", "0", "0");
		vector<Relation*> list_slave;
		DatabaseHandler::getInstance().getRelations(list_slave, ref_slave);
		for(Relation* n : list_slave) {
			r->addRelation(n->relation, getObjectPointer(n->pid), false);
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

	ObjectPointer* GameManager::getObjectPointer(objId id) {
		return new ObjectPointer(this, id);
	}

	void GameManager::insertObject(IObject* obj) {
        LOGS("GameManager", Verbose) << "Inserting object " << obj->getId() << "." << LOGF;
		objects.insert(obj);
		saveObject(obj);
	}
	
	void GameManager::saveObject(IObject* obj) {
        LOGS("GameManager", Verbose) << "Saving object " << obj->getId() << "." << LOGF;
		loader->saveObject(obj);
	}
	
	void GameManager::deleteObject(IObject* obj) {
		LOGS("GameManager", Verbose) << "Deleting object " << obj->getId() << "." << LOGF;
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

	void GameManager::addRelation(IObject* master, IObject* slave, string relation) {
		Relation* ref = new Relation(master->getId(), slave->getId(), master->className(), slave->className(), relation);
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
		createRelation(getObject("room/baseRoom"), figure, R_INSIDE);
		return figure;
    }
	
	void GameManager::createRelation(objId mid, objId sid, string mclass, string sclass, string relation) {
		Relation* rel = new Relation(mid, sid, mclass, sclass, relation);
		this->addRelation(rel);
		delete rel;
	}
	
	void GameManager::createRelation(IObject* master, IObject* slave, string relation) {
		this->createRelation(master->getId(), slave->getId(), master->className(), slave->className(), relation);
		master->addRelation(relation, this->getObjectPointer(slave->getId()));
		slave->addRelation(relation, this->getObjectPointer(master->getId()), false);
	}
	
	void GameManager::clearRelationsOfType(IObject* obj, string relation, bool master) {
		Relation* ref_obj;
		if (master)
			ref_obj = new Relation(obj->getId(), "0", "0", "0", relation);
		else ref_obj = new Relation("0", obj->getId(), "0", "0", relation);
		DatabaseHandler::getInstance().deleteRelation(ref_obj);
		delete ref_obj;
		
		obj->getRelations(master).erase(relation);
	}
	
	void GameManager::removeRelation(IObject* master, IObject* slave, string relation) {
 		master->eraseRelation(relation, this->getObjectPointer(slave->getId()));
 		slave->eraseRelation(relation, this->getObjectPointer(master->getId()), false);
		
		Relation* ref_obj = new Relation(master->getId(), slave->getId(), "0", "0", relation);
		DatabaseHandler::getInstance().deleteRelation(ref_obj);
		delete ref_obj;
	}
	
	void GameManager::moveAlive(Alive* alive, objId roomId) {
		this->removeRelation(alive->getLocation()->get(), alive, R_INSIDE);
		this->createRelation(this->getObject(roomId), alive, R_INSIDE);
	}


}
