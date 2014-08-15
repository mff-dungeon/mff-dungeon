#ifndef DOOR_HPP
#define	DOOR_HPP

#include "../common.hpp"

namespace Dungeon {

    /**
     * Basic mean of movement.
     */
    class Door : public IObject {
    public:
        Door();
        Door(objId id) : IObject(id) {}
        virtual void getActions(ActionList* list, IObject* callee);
        
        void goThrough(ActionDescriptor* ad);

        
    PERSISTENT_DECLARATION(Door, "Door")
    }; 
}

#endif	/* DOOR_HPP */

