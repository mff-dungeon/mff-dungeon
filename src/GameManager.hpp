#ifndef GAMEMANAGER_HPP
#define	GAMEMANAGER_HPP

#include "common.hpp"
#include "dynamic.hpp"
#include "SplayTree.hpp"
#include "ObjectLoader.hpp"
#include "ObjectPointer.hpp"
#include "DatabaseHandler.hpp"
#include "ActionQueue.hpp"

namespace Dungeon {
   
    class GameManager {
    private:
        SplayTree objects;
        ObjectLoader *loader;
        ActionQueue* aqueue;

    public:
        GameManager(bool init = false);
        void initWorld();
        IObject* getObject(objId id);
        bool hasObject(objId id);
        IObject* loadObject(objId id);
        void insertObject(IObject *object);
        void saveObject(IObject *object);
		void deleteObject(IObject *object);
        ObjectPointer* getObjectPointer(objId id);
		
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
		 * CAUTION! It won't publish the change to already loaded objects.
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
		 * Removes all the relations of given type
         * @param obj object to remove relations from
         * @param relation relations to be removed
         * @param master true, if the relation is a master relation
         */
        void clearRelationsOfType(IObject* obj, string relation, bool master = true);
		
		/**
		 * Removes a relation from the object and from database
         * @param master Pointer to the master object
         * @param slave Pointer to the slave object
         * @param relation type of removed relation
         */
        void removeRelation(IObject* master, IObject* slave, string relation);
        
        ActionQueue* getQueue();
        
        Alive* addNewFigure(Alive *figure);
        void moveAlive(Alive* alive, objId roomId);
        
        void shutdown();
    };
}


#endif

