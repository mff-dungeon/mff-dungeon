/*
 * The way objects interact with each other.
 */

#ifndef ACTION_HPP
#define	ACTION_HPP

#include "common.hpp"

namespace Dungeon {

    class Action {
    public:
        Action(string type) : type(type) {}
        virtual ~Action() { }
        virtual bool matchCommand(string command);
        virtual void commit(ActionDescriptor *ad);
        virtual void explain(ActionDescriptor *ad);

        string type;
    };
}

#endif	/* ACTION_HPP */

