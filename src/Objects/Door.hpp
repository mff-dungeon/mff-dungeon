#ifndef DOOR_HPP
#define	DOOR_HPP

#include "../common.hpp"
#include "IDescriptable.hpp"

namespace Dungeon {

    /**
     * Basic mean of movement.
     */
    class Door : public IDescriptable {
    public:
        Door();
        Door(objId id) : IDescriptable(id) {}
        virtual void getActions(ActionList* list, IObject* callee);
        
        void goThrough(ActionDescriptor* ad);
        virtual string getDescriptionSentence();
        
    PERSISTENT_DECLARATION(Door)
    }; 
}

#endif	/* DOOR_HPP */

