#include "../../common.hpp"

#ifndef IOBJECT_HPP
#define	IOBJECT_HPP

#include "../../Persistence/AddObject.hpp"
#include "../../Persistence/IPropertyStorage.hpp"
#include "../../Persistence/Relation.hpp"

/*
 * Macro for each saveable object - defines a function returning a new blank 
 *  object, defines static AddIObject (which registers the object) and 
 *  defines a className() method for saving purposes.
 * Should be used in class declaration.
 */
#define PERSISTENT_DECLARATION(cName, pName) \
public: \
	virtual Base* createObject() const \
	{ \
		return new cName(); \
	} \
private: \
	static AddObject addObject; \
        NONPERSISTENT_DECLARATION(cName, pName)


/*
 *	Simple line of code registering the object
 */
#define PERSISTENT_IMPLEMENTATION_FULL(fullName, cName) \
	AddObject fullName::addObject(#fullName, new fullName()); \
        NONPERSISTENT_IMPLEMENTATION(fullName, cName)

#define PERSISTENT_IMPLEMENTATION(cName) \
	PERSISTENT_IMPLEMENTATION_FULL(cName, cName)

#define NONPERSISTENT_DECLARATION(cName, pName) \
    public: \
	inline virtual const char * className() const { \
		return cName::cName##ClassName; \
	}; \
        const static char * cName##ClassName; \
	inline virtual bool isInstanceOf(char const * cname) const { \
		return cName##ClassName == cname || pName::isInstanceOf(cname); \
	}; \
    inline virtual void getActionsRecursive(ActionList* list, ObjectPointer callee) { \
        if (!cName##GettingActions) { /* Loop prevention */ \
            cName##GettingActions = true; \
            pName::getActionsRecursive(list, callee); \
            cName::getActions(list, callee); \
            cName##GettingActions = false; \
        } \
    } \
    private: \
        bool cName##GettingActions = false;

#define NONPERSISTENT_IMPLEMENTATION(fullName, cName) \
        const char * fullName::cName##ClassName = #fullName;

/**
 * This way we can call obj->instanceOf(IDescriptable) and it will work
 */
#define instanceOf(cName) isInstanceOf(cName::cName##ClassName)


namespace Dungeon {

    class ActionList;
    class ActionDescriptor;
    class Archiver;
    class GameManager;
    
    /**
     * Base class of everything in the world.
     */
    class Base : public IStorable {
        friend class GameManager; // Injecting GM
        friend struct ObjectPointer; // Access to GM when cloning
        friend class ObjectLoader; // Setting ID
        friend class Cloner; // Setting ID
    public:

        Base() {};

        Base(const objId& id) : id(id) {};

        virtual ~Base()
        {
            if (isLocked())
                LOGS(Error) << "Locked object is being deleted, something is wrong!" << LOGF;
        };

        virtual const objId& getId() const;
		virtual string getObjectType() const;

        /**
         * Which actions can be performed by callee on this object?
         * Do NOT call directly, use @see getActionsRecursive instead.
         */
        virtual void getActions(ActionList * list, ObjectPointer callee) = 0;

        /**
         * Which actions can be performed by callee on this object?
         */
        virtual void getActionsRecursive(ActionList * list, ObjectPointer callee);

        /**
         * Should be called in getActions whenever you want to delegate certain action.
         */
        void delegateGetActions(ActionList* list, ObjectPointer callee, std::initializer_list<const string> relations) const;

        /**
         * Should be called in getActions whenever you want to delegate certain action.
         */
        void delegateGetActions(ActionList* list, ObjectPointer callee, const string& relation) const;

        /*
         * Serializing functions: 
         *	createObject() returns a new blank object - class needs to define
         *   a constructor with no parameters
         *  load/store - not to be overwritten, handles saving/loading objects
         *  serialize - (deprecated @see registerProperties) must be overwritten, defines a way to serialize
         *  className - method defines by macro, see common.hpp
         */
        virtual Base* createObject() const = 0;
        static Base* load(Archiver& stream, const string& className);
        void store(Archiver& stream, objId& id, string& className) const;

        /**
         * Saves changes made to object. Must be called after changes.
         * Shortcut for gm->saveObject(obj).
         */
        ObjectPointer save();

        /*
         * Relation Functions:
         *  master relations - this object acts as the master
         *  slave relations - this object acts as the slave
         */

        /**
         * Checks whether this relation exists
         * @param type the type of the new relation
         * @param other ObjectPointer of the other object
         * @param master true, if the relation is master relation
         */
        bool hasRelation(const string& type, ObjectPointer other, Relation::Dir dir = Relation::Master);

        /**
         * Returns either master, or slave relations of the object
         * @param master true, if the relation is master relation
         */
        const RelationList& getRelations(Relation::Dir dir) const;

        /**
         * Returns objects with given relation to this object
         * @param master true, if the relation is master relation
         */
        const ObjectMap& getRelations(Relation::Dir dir, const string& type) const;

        /**
         * Returns the one and only relation of this type.
         * Throws GameStateInvalid when there are more of these.
         * @return nullptr when no such relation found.
         */
        ObjectPointer getSingleRelation(
                const string& type,
                Relation::Dir dir = Relation::Master,
                const string& errMsg = "There are more than one object for single relation.") const;

        /**
         * Sets the one and only relation of this type.
         * Throws GameStateInvalid when there are more of these.
         * @return itself
         */
        ObjectPointer setSingleRelation(
                const string& type,
                ObjectPointer other,
                Relation::Dir dir = Relation::Master,
                const string& errMsg = "There are more than one object for single relation.");

        /**
         * A special method used for some magic
         * @return the name of this class
         */
        inline virtual const char * className() const
        {
            return BaseClassName;
        };

        const static char * BaseClassName;

        /**
         * Returns, whether the current class is an instance of desired class
         * See cname limitations because of fast implementation.
         * @param cname MUST be pointer to <class>::<class>ClassName constant
         * @return true, if the class is instance of cname
         */
        inline virtual bool isInstanceOf(char const * cname) const
        {
            return BaseClassName == cname;
        };

        /**
         * Must be overwritten if there is some property 
         * that we want to be configurable. Handles both load&store
         * and in-game configuration.
         */
        virtual void registerProperties(IPropertyStorage& storage);

        /**
         * If the object is locked, it should not be deleted.
         */
        virtual bool isLocked()
        {
            return loadLock > 0;
        }
        
        /**
         * Shortcut for invoking all traps associated with this object for event
         * @param event identificator of event - be consistent
         * @param ad Is not required, but must be explicitly NULL
         */
        virtual ObjectPointer triggerTraps(const string& event, ActionDescriptor *ad);
        
        ObjectPointer attachTrap(ObjectPointer trap, const string& event);
        ObjectPointer detachTrap(ObjectPointer trap, const string& event);
        bool hasTrapAttached(ObjectPointer trap, const string& event);
        
        ObjectPointer deepClone() const;
        ObjectPointer shallowClone() const;
        

    protected:
        GameManager* getGameManager() const;
        void setGameManager(GameManager* gm);

    private:
        GameManager* gm;
        objId id;
        RelationList relation_master, relation_slave;

        int loadLock = 0;

        /**
         * Only some classes can set id
         */
        virtual Base * setId(objId id);

        /**
         * Erases a given relation of this object
         * @param type the type of the erased relation
         * @param other ObjectPointer of the other object
         * @param master true, if the relation is master relation
         */
        void eraseRelation(const string& type, ObjectPointer other, Relation::Dir dir);

        /**
         * Registers a new relation for this object
         * @param type the type of the new relation
         * @param other ObjectPointer of the other object
         * @param master true, if the relation is master relation
         */
        void addRelation(const string& type, ObjectPointer other, Relation::Dir dir);
    };
}

#endif
