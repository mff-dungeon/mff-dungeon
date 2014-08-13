#include <string>
#include <vector>
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
#define PERSISTENT_DECLARATION(cName, cStringName) \
public: \
	virtual IObject* createObject() const \
	{ \
		return new cName(); \
	} \
private: \
	static AddIObject addIObject; \
	virtual string className() const { \
		return cStringName; \
	};

/*
 *	Simple line of code registering the object
 */
#define PERSISTENT_IMPLEMENTATION(cName) \
	AddIObject cName::addIObject(#cName, new cName());

/*
 *  Logging
 */
#include "Logger.hpp"
#define LOG(source) ( Logger::getInstance().beginMessage(source), Logger::getInstance() )
#define LOGS(source, severity) ( Logger::getInstance().beginMessage(source, severity), Logger::getInstance() )
#define LOGH(headline) { Logger::getInstance().setHeadline(headline); } 
#define LOGF ( Logger::getInstance().endMessage(), "" )

using namespace std;

namespace Dungeon {
    /*
     * Circular dependencies...
     */
    class GameManager; 
    class ObjectLoader;
    class ObjectPointer;
    class Alive;
    class Action;
    class ActionDescriptor;
    class ActionQueue;
    class IObject;
    class Driver;
    class AddIObject;
    
    /**
     * Should be value type (struct), comparable
     */
    typedef string objId;
    typedef vector<Action *> ActionList;
}

#include "IObject.hpp"
#include "AddIObject.hpp"

#endif

