#ifndef IOBJECT_HPP
#define	IOBJECT_HPP

#include "common.hpp"
#include "Archiver.hpp"
#include "ObjectList.hpp"
#include "AddIObject.hpp"
#include "IPropertyStorage.hpp"

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
	virtual const char * className() const { \
		return cName::cName##ClassName; \
	}; \
        const static char * cName##ClassName; \
	virtual bool isInstanceOf(char const * cname) const { \
		if(cName::className() == cname) { \
			return true; \
		} else { \
			return pName::isInstanceOf(cname); \
		} \
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
    friend class GameManager;
    public:
        IObject() {};
        IObject(objId id) : id(id) {};
        virtual ~IObject() {};

        virtual objId getId() const;
        virtual IObject * setId(objId id);
        
        /**
         * Which actions can be performed by callee on this object?
         */
        virtual void getActions(ActionList * list, IObject *callee) = 0;
        
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
         * Registers a new relation for this object
         * @param type the type of the new relation
         * @param other ObjectPointer of the other object
         * @param master true, if the relation is master relation
         */
        void addRelation(string type, ObjectPointer other, bool master=true);
                
        /**
         * Checks whether this relation exists
         * @param type the type of the new relation
         * @param other ObjectPointer of the other object
         * @param master true, if the relation is master relation
         */
        bool hasRelation(string type, ObjectPointer other, bool master=true);
		
        /**
         * Erases a given relation of this object
         * @param type the type of the erased relation
         * @param other ObjectPointer of the other object
         * @param master true, if the relation is master relation
         */
        void eraseRelation(string type, ObjectPointer other, bool master=true);
		
        /**
         * Returns either master, or slave relations of the object
         * @param master true, if the relation is master relation
         */
        RelationList getRelations(bool master=true);
        
        /**
         * A special method used for some magic
         * @return the name of this class
         */
	virtual const char * className() const {
		return IObject::IObjectClassName;
	};
        
        const static char * IObjectClassName;
        
        /**
         * Returns, whether the current class is an instance of desired class
         * See cname limitations because of fast implementation.
         * @param cname MUST be pointer to <class>::<class>ClassName constant
         * @return true, if the class is instance of cname
         */
	virtual bool isInstanceOf(char const * cname) const {
		return IObjectClassName == cname;
	};

        
        ObjectPointer getObjectPointer();

        /**
         * Must be overwritten if there is some property 
         * that we want to be configurable. Handles both load&store
         * and in-game configuration.
         */
        virtual void registerProperties(IPropertyStorage& storage);
        
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
    };
}

#endif