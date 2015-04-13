
#ifndef GAMEMANAGER_HPP
#define	GAMEMANAGER_HPP

#include "../common.hpp"
#include "../Utils/SplayTree.hpp"
#include "../Objects/Alive.hpp"
#include "../Persistence/ObjectLoader.hpp"
#include "ObjectPointer.hpp"
#include "ActionQueue.hpp"

namespace Dungeon {

	class GameManager {
		friend struct ObjectPointer;

	public:

		enum GameMode {
			Normal = 0, // The basic intented game mode
			Hardcore = 1, // Hardcore mode, if you die, you must restart the game
			Test = 2 // Can implement testing specific features
		};
		GameManager(bool init = false);
		GameMode getGameMode() const;
		void initWorld(bool askOnConsole = true);
		bool hasObject(const objId& id);
		bool hasObjectLoaded(const objId& id);
		Base* loadObject(const objId& id);
		vector<objId> getObjectList();

		/**
		 * Inserts an object into world.
		 */
		void insertObject(Base *object);

		/**
		 * Propagates changes on the objects to the storage.
		 */
		void saveObject(Base *object);

		/**
		 * Deletes object from the world.
		 */
		void deleteObject(Base *object);

		/**
		 * Gets OP for an object.
		 * @param id
		 * @return 
		 */
		ObjectPointer getObject(const objId& id);

		/**
		 * Adds a relation to the database
		 * @param rel Relation to be added
		 */
		void addRelation(const Relation& rel);

		/**
		 * Checks if the relation exists either in the DB, or in the world
		 * @param master Master
		 * @param slave Slave
		 * @param relation Relation Type
		 * @return 
		 */
		bool hasRelation(ObjectPointer master, ObjectPointer slave, const string& relation);

		/**
		 * Creates relation on already loaded objects
		 * @param master Master
		 * @param slave Slave
		 * @param relation Relation type
		 */
		void createRelation(ObjectPointer master, ObjectPointer slave, const string& relation);

		/**
		 * Removes all the relations of given type.
		 * @param obj object to remove relations from
		 * @param relation relations to be removed
		 * @param master true, if the relation is a master relation
		 */
		void clearRelationsOfType(ObjectPointer obj, const string& relation, Relation::Dir master = Relation::Master);

		/**
		 * Removes a relation from the object and from storage
		 * @param master Pointer to the master object
		 * @param slave Pointer to the slave object
		 * @param relation type of removed relation
		 */
		void removeRelation(ObjectPointer master, ObjectPointer slave, const string& relation);

		ActionQueue* getQueue();

		/**
		 * Insert's figure to the world, gives it basic equipment 
		 * and put it somewhere
		 */
		Alive* addNewFigure(Alive *figure);

		/**
		 * Shortcut for moving an alive
		 * @param alive
		 * @param roomId Where to put the alive
		 */
		void moveAlive(Alive* alive, ObjectPointer room);

		/**
		 * Shutdown the world.
		 * TODO: Get reason, message active users...
		 */
		void shutdown();

		/**
		 * Handlers for counting time
		 */
		void roundBegin(ActionDescriptor* ad);
		void roundEnd(bool noException);

	protected:
		Base* getObjectInstance(const objId& id);

		/**
		 * Adds a relation to the database
		 * @param master Pointer to the master object
		 * @param slave Pointer to the slave object
		 * @param relation type of the relation
		 */
		void addRelation(ObjectPointer master, ObjectPointer slave, const string& relation);
	private:
		GameMode gameMode;
		SplayTree objects;
		ObjectLoader loader;
		ActionQueue* aqueue;
	};
}


#endif

