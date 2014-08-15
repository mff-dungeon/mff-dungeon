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
        void bigBang();
        
    private:
        GameManager* gm;
        Door* createDoor(string name, Room* a, Room* b);
    };

}

#endif	/* WORLDCREATOR_HPP */

