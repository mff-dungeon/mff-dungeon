/** 
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
#include "Action.hpp"

#define CALLBACK(className, methodName) std::bind(&className::methodName, this, std::placeholders::_1)
namespace Dungeon {

    class CallbackAction : public Action {
    public:
        CallbackAction(const string& type,
                const string& explanation,
                function<bool (string)> matchCallback,
                function<void (ActionDescriptor*)> commitCallback,
                bool visible = true)
            : Action(type, visible),
                explanation(explanation), 
                matchCallback(matchCallback), 
                commitCallback(commitCallback) {};

        virtual void explain(ActionDescriptor* ad);
        virtual void commit(ActionDescriptor* ad);
        virtual bool match(const string& command, ActionDescriptor* ad);

    private:
        string explanation;
        function<bool (string)> matchCallback;
        function<void (ActionDescriptor*)> commitCallback;
    };

}

#endif