#ifndef GAMEMANAGER_HPP
#define	GAMEMANAGER_HPP

#include "common.hpp"
#include "splayTree.hpp"
#include "ObjectLoader.hpp"
#include "ObjectPointer.hpp"

namespace Dungeon {
   
    class GameManager {
    private:
        SplayTree objects;
        ObjectLoader *loader;

    public:
        GameManager();
        IObject* getObject(objId id);
        void insertObject(IObject *object);
        ObjectPointer* getObjectPointer(objId id);
    };
}


#endif

