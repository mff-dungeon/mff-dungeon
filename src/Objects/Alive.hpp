#ifndef ALIVE_HPP
#define	ALIVE_HPP

#include "../common.hpp"
#include "../Actions/CallbackAction.hpp"


namespace Dungeon
{

    class Alive : public IObject
    {
    public:
        Alive(objId id) : IObject(id) {};
        virtual void getActions(ActionList* list, IObject *callee);
        int hitpoints;
    };

}

#endif	/* ALIVE_HPP */

