/* 
 * This should be an easy way to create use-once actions.
 * Just kinda magic. Desired usage:
 * 
 *  new CallbackAction("action-type", "Explanation", 
 *      RegexMatcher::matcher("^reg(ular)?ex(pression)?"),
 *      [] (ActionDescriptor* ad) { ... });
 */

#ifndef CALLBACKACTION_H
#define CALLBACKACTION_H

#include <functional>
#include "../common.hpp"
#include "../RegexMatcher.hpp"
#include "../Action.hpp"

namespace Dungeon {

    class CallbackAction : public Action {
    public:
        CallbackAction(string type,
                string explanation,
                function<bool (string)> matchCallback,
                function<void (ActionDescriptor*)> commitCallback) 
            : Action(type), 
                explanation(explanation), 
                matchCallback(matchCallback), 
                commitCallback(commitCallback) {};

        virtual void explain(ActionDescriptor* ad);
        virtual void commit(ActionDescriptor* ad);
        virtual bool matchCommand(string command);

    private:
        string explanation;
        function<bool (string)> matchCallback;
        function<void (ActionDescriptor*)> commitCallback;
    };

}

#endif