#include "common.hpp"

#ifndef IOBJECT_HPP
#define	IOBJECT_HPP

#include "Archiver.hpp"
#include "ObjectList.hpp"
#include "AddIObject.hpp"
#include "IPropertyStorage.hpp"
#include "Relation.hpp"

/*
 * Macro for each saveable object - defines a function returning a new blank 
 *  object, defines static AddIObject (which registers the object) and 
 *  defines a className() method for saving purposes.
 * Should be used in class declaration.
 */
#define PERSISTENT_DECLARATION(cName, pName) \
public: \
	virtual IObject* createObject() const \
	{ \
		return new cName(); \
	} \
private: \
	static AddIObject addIObject; \
        NONPERSISTENT_DECLARATION(cName, pName)
        

/*
 *	Simple line of code registering the object
 */
#define PERSISTENT_IMPLEMENTATION(cName) \
	AddIObject cName::addIObject(#cName, new cName()); \
        NONPERSISTENT_IMPLEMENTATION(cName)

#define NONPERSISTENT_DECLARATION(cName, pName) \
public: \
	inline virtual const char * className() const { \
		return cName::cName##ClassName; \
	}; \
        const static char * cName##ClassName; \
	inline virtual bool isInstanceOf(char const * cname) const { \
		return cName##ClassName == cname || pName::isInstanceOf(cname); \
	};

#define NONPERSISTENT_IMPLEMENTATION(cName) \
        const char * cName::cName##ClassName = #cName;

/**
 * This way we can call obj->instanceOf(IDescriptable) and it will work
 */
#define instanceOf(cName) isInstanceOf(cName::cName##ClassName)


namespace Dungeon {
    
    /**
     * Base clas of everything in the world.
     */
    class IObject : public IStorable {
    friend class GameManager; // Injecting GM
    friend struct ObjectPointer; // Access to GM when cloning
    friend class ObjectLoader; // Setting ID
    public:
        IObject() {};
        IObject(objId id) : id(id) {};
        virtual ~IObject() {
            if (isLocked())
                LOGS("IObject", Error) << "Locked object is being deleted, something is wrong!" << LOGF;
            LOG("IObject") << "Deleting " + id << LOGF;
        };

        virtual objId getId() const;
        
        /**
         * Which actions can be performed by callee on this object?
         */
        virtual void getActions(ActionList * list, ObjectPointer callee) = 0;
        
        /*
         * Serializing functions: 
         *	createObject() returns a new blank object - class needs to define
         *   a constructor with no parameters
         *  load/store - not to be overwritten, handles saving/loading objects
         *  serialize - (deprecated @see registerProperties) must be overwritten, defines a way to serialize
         *  className - method defines by macro, see common.hpp
         */
        virtual IObject* createObject() const = 0;
        static IObject* load(Archiver& stream, string className);
        void store(Archiver& stream, objId& id, string& className) const;
        
        /**
         * Saves changes made to object. Must be called after changes.
         * Shortcut for gm->saveObject(obj).
         */
        IObject* save();
		
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
        bool hasRelation(string type, ObjectPointer other, Relation::Dir dir = Relation::Master);
		
        /**
         * Returns either master, or slave relations of the object
         * @param master true, if the relation is master relation
         */
        RelationList& getRelations(Relation::Dir dir);
        
        /**
         * Returns objects with given relation to this object
         * @param master true, if the relation is master relation
         */
        ObjectMap& getRelations(Relation::Dir dir, string type);
        
        /**
         * Returns the one and only relation of this type.
         * Throws GameStateInvalid when there are more of these.
         * @return nullptr when no such relation found.
         */
        ObjectPointer getSingleRelation(
                string type, 
                Relation::Dir dir = Relation::Master, 
                string errMsg = "There are more than one object for single relation.");
        
        /**
         * Sets the one and only relation of this type.
         * Throws GameStateInvalid when there are more of these.
         * @return itself
         */
        ObjectPointer setSingleRelation(
                string type, 
                ObjectPointer other, 
                Relation::Dir dir = Relation::Master, 
                string errMsg = "There are more than one object for single relation.");
        
        /**
         * A special method used for some magic
         * @return the name of this class
         */
	inline virtual const char * className() const {
		return IObjectClassName;
	};
        
        const static char * IObjectClassName;
        
        /**
         * Returns, whether the current class is an instance of desired class
         * See cname limitations because of fast implementation.
         * @param cname MUST be pointer to <class>::<class>ClassName constant
         * @return true, if the class is instance of cname
         */
	inline virtual bool isInstanceOf(char const * cname) const {
		return IObjectClassName == cname;
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
        virtual bool isLocked() {
            return loadLock > 0;
        }
        
    protected:
        /**
         * Kept as warning :)
         */
        void serialize(Archiver& stream);
        GameManager* getGameManager() const;
        void setGameManager(GameManager* gm);
        	
    private:
        GameManager* gm;
        objId id;
	RelationList relation_master, relation_slave;
        
        int loadLock = 0;
        
        /**
         * Only GM can set id
         */
        virtual IObject * setId(objId id);
		
        /**
         * Erases a given relation of this object
         * @param type the type of the erased relation
         * @param other ObjectPointer of the other object
         * @param master true, if the relation is master relation
         */
        void eraseRelation(string type, ObjectPointer other, Relation::Dir dir);

        /**
         * Registers a new relation for this object
         * @param type the type of the new relation
         * @param other ObjectPointer of the other object
         * @param master true, if the relation is master relation
         */
        void addRelation(string type, ObjectPointer other, Relation::Dir dir);
    };
}

#endif