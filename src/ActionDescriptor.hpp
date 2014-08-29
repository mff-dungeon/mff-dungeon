#ifndef ACTIONDESCRIPTOR_HPP
#define	ACTIONDESCRIPTOR_HPP

#include <sstream>
#include <queue>
#include <functional>
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
        typedef function<void (ActionDescriptor*, string)> dialogReply;
        
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
        ActionDescriptor& operator<<(const string& msg);
        ActionDescriptor& operator<<(const int& msg);
        
        bool isValid(Driver* driver);
        stringstream messages;
        
        /**
         * Queries user for something. Givencallback will be called 
         * on next user's message INSTEAD of regular action processing.
         * @param callback Function receiving (AD*, string), where the string is user's reply.
         */
        void waitForReply(dialogReply callback) {
            dialogReplies.push(callback);
        }
        
        /**
         * Driver should only dispose this AD if it's finished. 
         * If it's not, i should keep it and call userReplied 
         * on user's response.
         * 
         * Driver MUST NOT BLOCK until response. It must remember 
         * this AD somewhere and load it later.
         */
        bool isFinished() {
            return dialogReplies.empty();
        }
        
        /**
         * If this action isn't finished yet, driver should call this function
         * whenever user types a message.
         * @param reply User's response
         */
        void userReplied(string reply) {
            dialogReply r = dialogReplies.front();
            dialogReplies.pop();
            r(this, reply);
        }
        
    private:
        Action* action;
        GameManager* gm;
        Alive* caller;
        int id;
        
        queue<dialogReply> dialogReplies;

    };
    
    /**
     * Prepared for distant future, where responses might be not only textual :)
     */
    class TextActionDescriptor : public ActionDescriptor {
    public:
        TextActionDescriptor(Driver * driver);
        string from, in_msg;
        
        string getReply();
        void clearReply();
        
    };
}

#endif

