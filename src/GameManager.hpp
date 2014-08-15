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
        ObjectPointer* getObjectPointer(objId id);
		
        void addRelation(Relation* rel);
        void createRelation(objId mid, objId sid, string mclass, string sclass, string relation);
        void createRelation(IObject* master, IObject* slave, string relation);
        void clearRelationsOfType(IObject* obj, string relation, bool master = true);
        
        ActionQueue* getQueue();
        
        void addNewFigure(Alive *figure);
        void moveAlive(Alive* alive, objId roomId);
        
        void shutdown();
    };
}


#endif

