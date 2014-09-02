#include <string>
#include <vector>
#include <map>
#include <iostream>

#ifndef COMMON_HPP
#define COMMON_HPP

#define DB_NAME "dungeon.db"

/*
 * Macro for each saveable object - defines a function returning a new blank 
 *  object, defines static AddIObject (which registers the object) and 
 *  defines a className() method for saving purposes.
 * Should be used in class declaration.
 */
#define PERSISTENT_DECLARATION(cName) \
public: \
	virtual IObject* createObject() const \
	{ \
		return new cName(); \
	} \
	virtual string className() const { \
		return #cName; \
	}; \
private: \
	static AddIObject addIObject;

/*
 *	Simple line of code registering the object
 */
#define PERSISTENT_IMPLEMENTATION(cName) \
	AddIObject cName::addIObject(#cName, new cName());

/*
 *	Simple macro for overloading instanceOf
 */
#define INSTANCEOF(cName, pName) \
public: \
	virtual bool instanceOf(std::string cname) const { \
		if(cname.compare(#cName) == 0) { \
			return true; \
		} else { \
			return pName::instanceOf(cname); \
		} \
	};

/**
 * Relation types used a lot
 */
// Alive is in a room, doors are in the room
#define R_INSIDE "inside"
// Alive has backpack, backpack has potion
#define R_INVENTORY "inventory"
// Door targets somewhere
#define R_TARGET "target"

/*
 *  Logging
 */
#include "Logger.hpp"
#define LOG(source) ( Logger::getInstance().beginMessage(source), Logger::getInstance() )
#define LOGS(source, severity) ( Logger::getInstance().beginMessage(source, Logger::Severity::severity), Logger::getInstance() )
#define LOGH(headline) { Logger::getInstance().setHeadline(headline); } 
#define LOGF ( Logger::getInstance().endMessage(), "" )

/*
 *  Random number generation
 */
#include <random>
#define rand_init(var, dist, min, max) std::default_random_engine var(time(0)); std::uniform_int_distribution<int> dist(min, max);
#define rand_next(var, dist) ( dist(var) )
#define RAND_ALNUM "qwertyuiopasdfghjklzxcvbnm0123456789"
#define RAND_ALNUM_LEN 36
#define RANDID Dungeon::Utils::getRandomString(6)

/*
 *  Helper functions for accessing parts of id
 */
#define objId_getType(id) ( id.substr(0, id.rfind("/")) )
#define objId_getIdentifier(id) ( id.substr(id.rfind("/") + 1) )

using namespace std;

namespace Dungeon {
    /*
     * Circular dependencies...
     */
    class GameManager; 
    class ObjectLoader;
    class ObjectGroup;
    class Alive;
    class Action;
    class ActionDescriptor;
    class ActionQueue;
    class IObject;
    class Driver;
    class AddIObject;
    class ActionList;
    class Item;
    class Potion;
    struct ObjectPointer;
    
    /**
     * Should be value type (struct), comparable
     */
    typedef string objId;
    typedef map<objId, ObjectPointer> ObjectMap;
    typedef map<string, ObjectMap> RelationList;
    
    class Utils {
    public:
        static string getRandomString(int len) {
            char ret [len + 1];
            ret[len] = 0;
            static rand_init(r, d, 0, RAND_ALNUM_LEN);
            for (int i = 0; i < len; ++i) ret[i] = RAND_ALNUM[rand_next(r, d)];
            return ret;
        }
    };
    
}
    
#include "ObjectPointer.hpp"
#include "IObject.hpp"
#include "AddIObject.hpp"



#endif

