#include <string>
#include <vector>
#include <map>
#include <iostream>

#ifndef COMMON_HPP
#define COMMON_HPP

#define GCC_VERSION ( __GNUC__ * 10000 \
					   + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__ )
#define CLANG_VERSION ( __clang_major__ * 10000 \
					   + __clang_minor__ * 100 + __clang_patchlevel__ )

// If the compiler used is not version 4.9+, or clang 3.2+, print warning on startup
// see main.cpp, main() function
#if GCC_VERSION > 40900
	#define COMPATIBLE
#elif CLANG_VERSION > 30200
	#define COMPATIBLE
#endif

/**********************************
 *				TO FIX
 **********************************
 *	-	Alive - getAllActions
 *	-	Creature - getActions
 *	-	HardcoreTrap - hardcore mode
 *	-	EquipAction::itemPhaseTwo 2x - checking the game state
 *	-	HealingTrap - the timestamp should be always updated 
 *					- atm, the user gets insta-healed after coming to healing location
 ***********************************/

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
#include "Utils/Config.hpp"

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

