#include <stdexcept>

#include "GameManager.hpp"

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
		/*
		 * Init Objects
		 */
		this->insertObject(new Room("room/baseRoom"));
		this->insertObject(new ThorsHammer());
		this->insertObject(new Alive("human/aearsis@eideo.cz"));
		this->insertObject(new Alive("human/asaru@jabbim.cz"));
		this->insertObject(new Alive("human/petr.manek@jabbim.com"));
		/*
		 * Init relations
		 */
		Relation* rel;
		rel = new Relation("room/baseRoom", "human/aearsis@eideo.cz", "Room", "Alive", "inside");
		this->addRelation(rel);
		delete rel;
		rel = new Relation("room/baseRoom", "human/asaru@jabbim.cz", "Room", "Alive", "inside");
		this->addRelation(rel);
		delete rel;
		rel = new Relation("room/baseRoom", "human/petr.manek@jabbim.com", "Room", "Alive", "inside");
		this->addRelation(rel);
		delete rel;
		rel = new Relation("human/aearsis@eideo.cz", "ThorsHammer", "Alive", "ThorsHammer", "inventory");
		this->addRelation(rel);
		delete rel;
		rel = new Relation("human/asaru@jabbim.cz", "ThorsHammer", "Alive", "ThorsHammer", "inventory");
		this->addRelation(rel);
		delete rel;
		rel = new Relation("human/petr.manek@jabbim.com", "ThorsHammer", "Alive", "ThorsHammer", "inventory");
		this->addRelation(rel);
		delete rel;
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
		IObject* r = 0;
		r = loader->loadObject(id);
		if(r == 0) return 0;
		// Load the relations
		Relation* ref_master = new Relation(id, "0", "0", "0", "0");
		vector<Relation*> list_master;
		DatabaseHandler::getInstance().getRelations(list_master, ref_master);
		for(Relation* n : list_master) {
			r->addRelation(n->relation, new ObjectPointer(this, n->sid), true);
			delete n;
		}
		delete ref_master;
		
		Relation* ref_slave = new Relation("0", id, "0", "0", "0");
		vector<Relation*> list_slave;
		DatabaseHandler::getInstance().getRelations(list_slave, ref_slave);
		for(Relation* n : list_slave) {
			r->addRelation(n->relation, new ObjectPointer(this, n->pid), false);
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
		objects.insert(obj);
		loader->saveObject(obj);
	}
	
	void GameManager::addRelation(Relation* rel) {
		int err = DatabaseHandler::getInstance().addRelation(rel);
        if(err != DatabaseHandler::E_OK) {
			LOGS("GameManager", Fatal) << "Error adding relation to database, error code " << err << LOGF;
		}
	}
	
	ActionQueue* GameManager::getQueue() {
		return this->aqueue;
	}
	
	void GameManager::shutdown() {
		this->aqueue->stop();
	}
    
    void GameManager::addNewFigure(Alive *figure) {
        // TODO: put the figure somewhere, initialize the inventory, and so on...
        
        this->insertObject(figure);
    }
}
