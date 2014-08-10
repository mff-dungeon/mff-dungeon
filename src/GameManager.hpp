#ifndef GAMEMANAGER_HPP
#define	GAMEMANAGER_HPP

#include "common.hpp"
#include "SplayTree.hpp"
#include "ObjectLoader.hpp"
#include "ObjectPointer.hpp"
#include "ActionQueue.hpp"

namespace Dungeon {
   
    class GameManager {
    private:
        SplayTree objects;
        ObjectLoader *loader;
        ActionQueue* aqueue;

    public:
        GameManager();
        IObject* getObject(objId id);
        void insertObject(IObject *object);
        ObjectPointer* getObjectPointer(objId id);
        
        ActionQueue* getQueue();
        
        void shutdown();
    };
}


#endif

