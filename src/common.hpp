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
#define PERSISTENT_DECLARATION(cName) \
public: \
	virtual IObject* createObject() const \
	{ \
		return new cName(); \
	} \
private: \
	static AddIObject addIObject; \
	virtual string className() const { \
		return "cName"; \
	};

/*
 *	Simple line of code registering the object
 */
#define PERSISTENT_IMPLEMENTATION(cName) \
	AddIObject cName::addIObject(#cName, new cName());


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

