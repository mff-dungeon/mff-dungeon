#ifndef IOBJECT_HPP
#define	IOBJECT_HPP

#include "common.hpp"
#include "Action.hpp"

namespace Dungeon {
    class IObject {
    public:
        IObject() {};
        IObject(objId id) : id(id) {};

        virtual objId getId();
        virtual IObject * setId(objId id);
        virtual void getActions(ActionList * list, IObject *callee) = 0;
       
    private:
        objId id;
    };
}

#endif

