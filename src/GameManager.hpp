#ifndef GAMEMANAGER_HPP
#define	GAMEMANAGER_HPP

#include "common.hpp"
#include "SplayTree.hpp"
#include "ObjectLoader.hpp"
#include "ObjectPointer.hpp"
#include "DatabaseHandler.hpp"

namespace Dungeon {
   
    class GameManager {
    private:
        SplayTree objects;
        ObjectLoader *loader;
        ActionQueue* aqueue;

    public:
        GameManager(bool init = false);
        void initWorld(bool askOnConsole = true);
        IObject* getObject(objId id);
        bool hasObject(objId id);
        bool hasObjectLoaded(objId id);
        IObject* loadObject(objId id);
        
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
        void addRelation(IObject* master, IObject* slave, string relation);
		
        /**
         * Creates relation. 
         * CAUTION! It won't propagate the change to already loaded objects.
         * Use the other version instead.
         * @param mid ID of the master
         * @param sid ID of the slave
         * @param mclass Class name of the master
         * @param sclass Class name of the slave
         * @param relation Relation type
         */
        void createRelation(objId mid, objId sid, string mclass, string sclass, string relation);
		
        /**
         * Creates relation on already loaded objects
         * @param master Master
         * @param slave Slave
         * @param relation Relation type
         */
        void createRelation(IObject* master, IObject* slave, string relation);
		
        /**
         * Removes all the relations of given type.
         * @param obj object to remove relations from
         * @param relation relations to be removed
         * @param master true, if the relation is a master relation
         */
        void clearRelationsOfType(IObject* obj, string relation, bool master = true);
		
        /**
         * Removes a relation from the object and from storage
         * @param master Pointer to the master object
         * @param slave Pointer to the slave object
         * @param relation type of removed relation
         */
        void removeRelation(IObject* master, IObject* slave, string relation);
        
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
        void moveAlive(Alive* alive, objId roomId);
        
        /**
         * Shutdown the world.
         * TODO: Get reason, message active users...
         */
        void shutdown();
    };
}


#endif

