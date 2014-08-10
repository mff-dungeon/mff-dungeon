/* 
 * Contains information about commiting an action.
 * It is created when user types the command, then 
 * waiting in queue, when commited it is filled with 
 * information to be returned to the user.
 */

#ifndef ACTIONDESCRIPTOR_HPP
#define	ACTIONDESCRIPTOR_HPP

#include <vector>
#include "common.hpp"
#include "Driver.hpp"

namespace Dungeon {
    
    class ActionDescriptor {
    public:
        ActionDescriptor(Driver * driver);
        Action* getAction();
        Alive* getAlive();
        GameManager* getGM();
        
        Driver* driver;
        
        void enqueued(GameManager* gm);
        void assigned(Alive* alive);
        void matched(Action* action);
        
        void addMessage(string msg);
        
        bool isValid(Driver* driver);
        vector<string> messages;
    private:
        Action* action;
        GameManager* gm;
        Alive* caller;
        int id;

    };
    
    class TextActionDescriptor : public ActionDescriptor {
    public:
        TextActionDescriptor(Driver * driver);
        string from, in_msg;
        
        string getReply();
        
    };
}

#endif

