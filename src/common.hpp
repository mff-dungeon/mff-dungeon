#include <string>
#include <vector>
#include <map>
#include <iostream>

#ifndef COMMON_HPP
#define COMMON_HPP

#define DB_NAME "dungeon.db"
#define CONFIG_GAME_MODE 0

/**
 * Relation types used a lot
 */
// Alive is in a room, doors are in the room
#define R_INSIDE "inside"
// Alive has backpack, backpack has potion
#define R_INVENTORY "inventory"
// Door targets somewhere
#define R_TARGET "target"
// Creature drops an item
#define R_DROP "dropper"
// Creature drops an item
#define R_RECIPE "crafter-recipe"
// Something requires a stat
#define R_REQUIREMENT "stat-req"
// Alive has gold
#define R_RESOURCE(type) ( "resource-" + (string)(Resource::ResourceIdentifier[(int)type]) )
// Alive explores stuff
#define R_SEEN "seen"


/*
 *  Logging
 */
#include "Utils/Logger.hpp"

#include "Utils/Utils.hpp"

namespace Dungeon {    

    struct ObjectPointer;
    
    typedef string objId;
    typedef map<objId, ObjectPointer> ObjectMap;
    typedef map<string, ObjectMap> RelationList;
    
}
    
#include "exceptions.hpp"
#include "Objects/Virtual/Base.hpp"
#include "Game/ObjectPointer.hpp"

#endif

