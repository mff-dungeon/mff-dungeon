#ifndef IOBJECT_HPP
#define	IOBJECT_HPP

#include "common.hpp"
#include "Archiver.hpp"
#include "ObjectList.hpp"
#include "AddIObject.hpp"

namespace Dungeon {
    class IObject {
    public:
        IObject() {};
        IObject(objId id) : id(id) {};
        virtual ~IObject() {};

        virtual objId getId() const;
        virtual IObject * setId(objId id);
        virtual void getActions(ActionList * list, IObject *callee) = 0;
       
        /*
         * Serializing functions: 
         *	createObject() returns a new blank object - class needs to define
         *   a constructor with no parameters
         *  load/store - not to be overwritten, handles saving/loading objects
         *  serialize - must be overwritten, defines a way to serialize
         *  className - method defines by macro, see common.hpp
         */
        virtual IObject* createObject() const = 0;
        static IObject* load(Archiver& stream, string className);
        void store(Archiver& stream, objId& id, string& className) const;
		
        /*
         * Relation Functions:
         *  master relations - this object acts as the master
         *  slave relations - this object acts as the slave
         */

        /**
         * Registers a new relation for this object
         * @param type the type of the new relation
         * @param other objectId of the other object
         * @param master true, if the relation is master relation
         */
	void addRelation(string type, ObjectPointer* other, bool master=true);
		
        /**
         * Returns either master, or slave relations of the object
         * @param master true, if the relation is master relation
         */
        RelationList getRelations(bool master=true);
        
        
                virtual string className() const = 0;

	protected:
		virtual void serialize(Archiver& stream);
		
    private:
        objId id;
	RelationList relation_master, relation_slave;
    };
}

#endif