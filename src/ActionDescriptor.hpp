#ifndef ACTIONDESCRIPTOR_HPP
#define	ACTIONDESCRIPTOR_HPP

#include <sstream>
#include <queue>
#include <functional>
#include "common.hpp"
#include "Driver.hpp"
#include "RandomString.hpp"
#include "Objects/Human.hpp"

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
        virtual ~ActionDescriptor();

        Action* getAction();
        Human* getCaller();
        GameManager* getGM();
        
        Driver* driver;
        
        string in_msg;
        
        enum State {
            RoundBegin,
            Round,
            RoundEnd,
            Trap
        } state = RoundBegin;
        
        enum ReplyFormat {
            Paragraph,
            List
        };
        
        /** Special type to indicate EOS - @see eos*/
        class EndOfSentence { };
        
        /**
         * Id called after receiving a message.
         */
        void enqueued(GameManager* gm);
        
        /**
         * Is called when there's a human to take responsibility.
         */
        void assigned(Human* human);
        
        /**
         * Is called when appropriate action is matched.
         */
        void matched(Action* action);
        
        /**
         * Sets the action for this AD.
         * It will be automatically deleted after replacing or deletign AD.
         * @param del To avoid deleting the action
         */
        void setAction(Action* action);
        
        /**
         * Add some text to the reply. @see operator <<
         */
        void addSentence(string msg);
        
        /**
         * Shortcut for addMessage.
         */
        ActionDescriptor& operator<<(const string& msg);
        ActionDescriptor& operator<<(const char * msg);
        ActionDescriptor& operator<<(const int& msg);
        
        /**
         * Finishes current sentence. @see eos
         */
        ActionDescriptor& operator<<(ActionDescriptor::EndOfSentence* (*endofsentence)());
        
        bool isValid(Driver* driver);
        
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
        
        void clearDialog() {
            queue<dialogReply>().swap(dialogReplies); // Trick
        }
        
        ActionDescriptor* setReplyFormat(ReplyFormat format);
        
    protected:
        string message;
        
    private:
        Action* action;
        GameManager* gm;
        Human* caller;
		ReplyFormat replyFormat = ReplyFormat::Paragraph;
        int id;
        
        queue<dialogReply> dialogReplies;
        
        int sentences = 0;
        stringstream currentSentence;
    };
    
    /**
      * Special value that breaks the stream and divides distinct sentences.
      * Do not call it, it makes sense only in stream context of ActionDescriptor.
      * @return NULL
      */
    ActionDescriptor::EndOfSentence* eos();
    
    /**
     * Prepared for distant future, where responses might be not only textual :)
     */
    class TextActionDescriptor : public ActionDescriptor {
    public:
        TextActionDescriptor(Driver * driver);
        string from;
        
        string getReply();
        void clearReply();
        
    };
}

#endif

