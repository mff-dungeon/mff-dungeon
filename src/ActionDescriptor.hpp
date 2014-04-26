/* 
 * Contains information about commiting an action.
 * It is created when user types the command, then 
 * waiting in queue, when commited it is filled with 
 * information to be returned to the user.
 */

#ifndef ACTIONDESCRIPTOR_HPP
#define	ACTIONDESCRIPTOR_HPP

class ActionDescriptor;
#include "common.hpp"
#include "Action.hpp"
#include "GameManager.hpp"

namespace Dungeon {
    class ActionDescriptor {
    public:
        ActionDescriptor(Action *action, GameManager *gm);
        Action *getAction();
        string message; // FIXME
        
    private:
        Action *action;
        GameManager *gm;
        int id;

    };
}

#endif

