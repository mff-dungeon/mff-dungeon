#include <string>
#include <vector>
#include <iostream>

#ifndef COMMON_HPP
#define COMMON_HPP
#define DB_NAME "data.db"

using namespace std;

namespace Dungeon {
    /*
     * Circular dependencies...
     */
    class GameManager; 
    class ObjectLoader;
    class ObjectPointer;
    class Action;
    class ActionDescriptor;
    class IObject;
    
    /**
     * Should be value type (struct), comparable
     */
    typedef string objId;
    typedef vector<Action *> ActionList;
}

#include "IObject.hpp"

#endif

