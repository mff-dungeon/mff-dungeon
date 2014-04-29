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
        void getAllActions(ActionList* list);
        virtual void getActions(ActionList* list, IObject *callee);
        int hitpoints;
    };

}

#endif	/* ALIVE_HPP */

