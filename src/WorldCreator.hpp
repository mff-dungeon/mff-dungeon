#ifndef WORLDCREATOR_HPP
#define	WORLDCREATOR_HPP

#include "common.hpp"
#include "dynamic.hpp"

namespace Dungeon {
    /**
     * Dedicated static class to init the world
     */
    class WorldCreator {
    public:
        WorldCreator(GameManager* gm);
        void initAdmins();
        virtual void bigBang();
        
    protected:
        GameManager* gm;
        Door* createDoor(string name, Room* a, Room* b, bool two_way = true);
        
        template<typename T>
        T* createObject(string id, IObject* location = NULL) {
            T* ptr = new T(id);
            gm->insertObject(ptr);
            if (location) gm->createRelation(location, ptr, R_INSIDE);
            return ptr;
        }
    };

}

#endif	/* WORLDCREATOR_HPP */

