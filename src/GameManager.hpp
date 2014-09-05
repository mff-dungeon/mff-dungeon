#ifndef GAMEMANAGER_HPP
#define	GAMEMANAGER_HPP

#include "common.hpp"
#include "SplayTree.hpp"
#include "ObjectLoader.hpp"
#include "ObjectPointer.hpp"
#include "DatabaseHandler.hpp"

namespace Dungeon {
   
    class GameManager {
        friend struct ObjectPointer;
    private:
        SplayTree objects;
        ObjectLoader *loader;
        ActionQueue* aqueue;

    public:
        GameManager(bool init = false);
        void initWorld(bool askOnConsole = true);
        bool hasObject(objId id);
        bool hasObjectLoaded(objId id);
        IObject* loadObject(objId id);
        vector<objId> getObjectList();
        
        /**
         * Inserts an object into world.
         */
        void insertObject(IObject *object);
        
        /**
         * Propagates changes on the objects to the storage.
         */
        void saveObject(IObject *object);
        
        /**
         * Deletes object from the world.
         */
        void deleteObject(IObject *object);
        
        /**
         * Gets OP for an object.
         * @param id
         * @return 
         */
        ObjectPointer getObjectPointer(objId id);
		
        /**
         * Adds a relation to the database
         * @param rel Relation to be added
         */
        void addRelation(Relation* rel);
		
        /**
         * Adds a relation to the database
         * @param master Pointer to the master object
         * @param slave Pointer to the slave object
         * @param relation type of the relation
         */
        void addRelation(ObjectPointer master, ObjectPointer slave, string relation);
	
        /**
         * Creates relation on already loaded objects
         * @param master Master
         * @param slave Slave
         * @param relation Relation type
         */
        void createRelation(ObjectPointer master, ObjectPointer slave, string relation);
		
        /**
         * Removes all the relations of given type.
         * @param obj object to remove relations from
         * @param relation relations to be removed
         * @param master true, if the relation is a master relation
         */
        void clearRelationsOfType(ObjectPointer obj, string relation, Relation::Dir master = Relation::Master);
		
        /**
         * Removes a relation from the object and from storage
         * @param master Pointer to the master object
         * @param slave Pointer to the slave object
         * @param relation type of removed relation
         */
        void removeRelation(ObjectPointer master, ObjectPointer slave, string relation);
        
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
    protected:
        
        IObject* getObject(objId id);
    };
}


#endif

