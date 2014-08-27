#ifndef DOOR_HPP
#define	DOOR_HPP

#include "../common.hpp"
#include "IDescriptable.hpp"
#include "../Actions/MultiTargetAction.hpp"

namespace Dungeon {

    /**
     * Basic mean of movement.
     */
    class Door : public IDescriptable {
    public:
        Door();
        Door(objId id) : IDescriptable(id) {}
        virtual void getActions(ActionList* list, IObject* callee);
        
        virtual string getDescriptionSentence();
        virtual string getGroupDescriptionSentence(vector<IDescriptable *> others);
        
    PERSISTENT_DECLARATION(Door)
    }; 
    
    class DoorwalkAction : public MultiTargetAction {
    public:
        DoorwalkAction(string type = "door-walk") : MultiTargetAction(type) {}
        
        virtual void explain(ActionDescriptor* ad);
        virtual bool matchCommand(string command);
        virtual void commit(ActionDescriptor* ad);
        virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer* target);
    };
}

#endif	/* DOOR_HPP */
