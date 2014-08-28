#ifndef ACTIONDESCRIPTOR_HPP
#define	ACTIONDESCRIPTOR_HPP

#include <sstream>
#include "common.hpp"
#include "Driver.hpp"

namespace Dungeon {
    
    /* 
    * Contains information about commiting an action.
    * It is created when user types the command, then 
    * waiting in queue, when commited it is filled with 
    * information to be returned to the user.
    */
    class ActionDescriptor {
    public:
        ActionDescriptor(Driver * driver);
        Action* getAction();
        Alive* getAlive();
        GameManager* getGM();
        
        Driver* driver;
        
        /**
         * Id called after receiving a message.
         */
        void enqueued(GameManager* gm);
        
        /**
         * Is called when there's an alive to take responsibility.
         */
        void assigned(Alive* alive);
        
        /**
         * Id called when appropriate action is matched.
         */
        void matched(Action* action);
        
        /**
         * Add some text to the reply. @see operator <<
         */
        void addMessage(string msg);
        
        /**
         * Shortcut for addMessage.
         */
        ActionDescriptor& operator<<(string msg) {
            addMessage(msg);
            return *this;
        }
        
        bool isValid(Driver* driver);
        stringstream messages;
        
    private:
        Action* action;
        GameManager* gm;
        Alive* caller;
        int id;

    };
    
    /**
     * Prepared for distant future, where responses might be not only textual :)
     */
    class TextActionDescriptor : public ActionDescriptor {
    public:
        TextActionDescriptor(Driver * driver);
        string from, in_msg;
        
        string getReply();
        
    };
}

#endif

