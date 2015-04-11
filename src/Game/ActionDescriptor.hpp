#ifndef ACTIONDESCRIPTOR_HPP
#define	ACTIONDESCRIPTOR_HPP

#include <sstream>
#include <queue>
#include <stack>
#include <functional>
#include "../common.hpp"
#include "../Game/ActionList.hpp"
#include "../Drivers/Driver.hpp"
#include "../Objects/Human.hpp"

namespace Dungeon {

class ActionQueue;

/* 
 * Contains information about commiting an action.
 * It is created when user types the command, then 
 * waiting in queue, when commited it is filled with 
 * information to be returned to the user.
 */
class ActionDescriptor {
public:
    typedef function<void (ActionDescriptor*, const string&) > dialogReply;

    ActionDescriptor(Driver * driver);
    virtual ~ActionDescriptor();

    ActionDescriptor& operator=(const ActionDescriptor& right);

    Action* getAction();
    Human* getCaller();
    GameManager* getGM();

    Driver* driver;
    
    /**
     * While getting actions, this is used to know who delegates
     */
    stack<ObjectPointer> delegationStack;
    
    string in_msg;

    enum State {
        Empty, // No useful information held
        ActionsFilled, // After getting actions
        ActionReady, // While matching actions
        ActionRunning, // Running the matched action
        ActionFinished, // Running the matched action
        Waiting, // Doing nothing, but following action exists
        Trapped, // Trap invoked
        Error, // Exception thrown
        Finished, // Can be disposed
        Invalid // WTF?
    } state = Empty;
    

    enum ReplyFormat {
        Paragraph,
        List
    };

    /** Special type to indicate EOS - @see eos*/
    class EndOfSentence {
    };

    /**
     * Id called after receiving a message.
     */
    void enqueued(GameManager* gm);

    /**
     * Is called when there's a human to take responsibility.
     */
    void assigned(ObjectPointer human);

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
     * Shortcut for addMessage. Do not try to template this,
     * e.g. RandomString needs to be explicitly converted to string.
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
     * Queries user for something. Given callback will be called 
     * on next user's message INSTEAD of regular action processing.
     * @param callback Function receiving (AD*, string), where the string is user's reply.
     */
    void waitForReply(dialogReply callback)
    {
        LOGS(Debug) << "Requested a dialog reply for user " << caller << "." << LOGF;
        dialogReplies.push(callback);
    }
    
    /**
     * Some gray magic here :)
     * 
     * Example: 
     *   ad->waitForReply(this, &CombatAction::combatLoop);
     * 
     * @param this_ptr "this"
     * @param callback &SomeClass::someFunction
     */
    template <typename T>
    void waitForReply(T* this_ptr, void (T::* callback)(ActionDescriptor *, const string &)) {
        LOGS(Debug) << "Requested a dialog reply for user " << caller << "." << LOGF;
        dialogReplies.push(bind(mem_fn(callback), this_ptr, placeholders::_1, placeholders::_2));
    }

    /**
     * Driver should only dispose this AD if it's finished.
     * If it's not, i should keep it and call userReplied
     *  on user's response.
     *
     * Driver MUST NOT BLOCK until response. It must remember
     * this AD somewhere and load it later.
     */
    bool isFinished()
    {
        return dialogReplies.empty();
    }

    /**
     * If this action isn't finished yet, driver should call this function
     * whenever user types a message.
     * @param reply User's response
     */
    void userReplied(string reply)
    {
        dialogReply r = dialogReplies.front();
        dialogReplies.pop();
        r(this, reply);
    }

    void clearDialog()
    {
        queue<dialogReply>().swap(dialogReplies); // Trick
    }

    ActionDescriptor* setReplyFormat(ReplyFormat format);

    /** 
     * Printf-like formatted message will be replaced as following:
     *      %u -> Username
     *      %% -> %
     * 
     * Obviously, more replacements will be added in time of development.
     */
    string formatMessage(string msg);

    ActionList& getActionList()
    {
        return alist;
    }
    
    void prepareFilling() {
        alist.clear();
        stack<ObjectPointer>().swap(delegationStack); // Clear the stack
        delegationStack.push(caller);
    }

protected:
    string message;

private:
    Action* action;
    GameManager* gm;
    ObjectPointer caller;
    ActionList alist;
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

