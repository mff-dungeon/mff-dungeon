#ifndef ALIVE_HPP
#define	ALIVE_HPP

#include "../common.hpp"
#include "../Actions/CallbackAction.hpp"


namespace Dungeon
{

    class Alive : public IObject
    {
    public:
        Alive() {};		// Constructor allowing to load class later
        Alive(objId id) : IObject(id) {};
        void getAllActions(ActionList* list);
        virtual void getActions(ActionList* list, IObject *callee);
        int hitpoints;
	
	protected:
		virtual void serialize(Archiver& stream);
	
	PERSISTENT_DECLARATION(Alive)		// Macro registering the object into list
    };

}

#endif	/* ALIVE_HPP */

