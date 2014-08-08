/* 
 * Contains information about commiting an action.
 * It is created when user types the command, then 
 * waiting in queue, when commited it is filled with 
 * information to be returned to the user.
 */

#ifndef ACTIONDESCRIPTOR_HPP
#define	ACTIONDESCRIPTOR_HPP

#include "common.hpp"
#include "Action.hpp"
#include "GameManager.hpp"

namespace Dungeon {
	class Alive;
    class ActionDescriptor {
    public:
        ActionDescriptor(Action* action, GameManager* gm, Alive* caller);
        Action* getAction();
		Alive* getAlive();
        string message; // FIXME
        
    private:
        Action* action;
        GameManager* gm;	// FIXME - not yet used
		Alive* caller;
        int id;

    };
}

#endif

