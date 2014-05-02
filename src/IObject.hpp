#ifndef IOBJECT_HPP
#define	IOBJECT_HPP

#include "common.hpp"
#include "Action.hpp"
#include "Archiver.hpp"
#include "ObjectList.hpp"
#include "AddIObject.hpp"

namespace Dungeon {
    class IObject {
    public:
        IObject() {};
        IObject(objId id) : id(id) {};

        virtual objId getId();
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

	protected:
		virtual string className() const = 0;
		virtual void serialize(Archiver& stream) = 0;
		
    private:
        objId id;
    };
}

#endif