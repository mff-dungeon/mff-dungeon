#ifndef ACTION_HPP
#define	ACTION_HPP

#include "common.hpp"

namespace Dungeon {
    
    /*
     * The way objects interact with each other.
     */
    class Action {
    public:
        Action(string type, bool visible = true) : type(type), isVisibleInHelp(visible) {}
        virtual ~Action() { }
        
        /**
         * Returns true when this action should be commited for this message.
         */
        virtual bool matchCommand(string command);
        
        /**
         * Commits the action.
         */
        virtual void commit(ActionDescriptor *ad);
        
        /**
         * Used in help text. Should fill the reply 
         * in ad with some useful information.
         * 
         * If this action should not be visible, then just do nothing.
         */
        virtual void explain(ActionDescriptor *ad);
        
        /**
         * Handles arosen in-game exception. Can be called only after matching,
         * or in dialog (where most errors can occur).
         * @param exception
         * @return true, if the error was handled properly and user
         *         should not be notified any further
         */
        virtual bool handleException(GameException& exception, ActionDescriptor *ad);

        string type;
        
        bool isVisibleInHelp;
    };
}

#endif	/* ACTION_HPP */

