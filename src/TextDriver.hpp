#ifndef TEXTDRIVER_HPP
#define	TEXTDRIVER_HPP

#include <sstream>
#include <cstdlib>
#include "common.hpp"
#include "Driver.hpp"
#include "Action.hpp"
#include "ActionQueue.hpp"
#include "Objects/Alive.hpp"
#include "ActionList.hpp"

namespace Dungeon {
    
    /**
     * Common class for text-communicating drivers.
     * Matches actions to strings, replies in string.
     */
    class TextDriver : public Driver {
    public:
        TextDriver(ActionQueue* queue);
        virtual ~TextDriver();
        
        bool process(TextActionDescriptor* ad);
        
        /**
         * Get some fancy response to unmatchable input
         */
        string getDontUnderstandResponse(string input);
        
        /**
         * Action for user's first interaction
         */
        Action* getCreateAction();
    protected:
        map<objId, TextActionDescriptor*> dialogs;
        
        bool isInDialog(objId who) {
            return dialogs.find(who) != dialogs.end();
        }
        
    private:
	ActionList alist;
    };
}
#endif

