/* 
 * This should be an easy way to create use-once actions.
 * Just kinda magic.
 */

#ifndef CALLBACKACTION_H
#define CALLBACKACTION_H

#include <functional>
#include "../common.hpp"
#include "../RegexMatcher.cpp"

namespace Dungeon {

    class CallbackAction : public Action {
    public:
        CallbackAction(string type,
                string explanation,
                bool (*matchCallback)(string command),
                function<void(ActionDescriptor*)> commitCallback) 
            : Action(type), 
                explanation(explanation), 
                matchCallback(matchCallback), 
                commitCallback(commitCallback) {};

        virtual void explain(ActionDescriptor* ad);
        virtual void commit(ActionDescriptor* ad);
        virtual bool matchCommand(string command);

    private:
        string explanation;
        bool (*matchCallback)(string);
        function<void (ActionDescriptor*)> commitCallback;
    };

}

#endif