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
        void insertObject(IObject *object);
        ObjectPointer* getObjectPointer(objId id);
		
		void addRelation(Relation* rel);
		vector<objId> getRelationIds(Relation* rel);
        
        ActionQueue* getQueue();
        
        void addNewFigure(Alive *figure);
        
        void shutdown();
    };
}


#endif

