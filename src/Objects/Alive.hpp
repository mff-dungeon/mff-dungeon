#ifndef ALIVE_HPP
#define	ALIVE_HPP

#include "../common.hpp"
#include "../Actions/CallbackAction.hpp"
#include "IDescriptable.hpp"


namespace Dungeon
{

    class Alive : public IDescriptable
    {
    public:
        Alive() {}		// Constructor allowing to load class later
        Alive(objId id);
        void getAllActions(ActionList* list);
        virtual void getActions(ActionList* list, IObject *callee);
        int hitpoints;
        
        virtual string getDescriptionSentence();
	
	protected:
		virtual void serialize(Archiver& stream);
	
	PERSISTENT_DECLARATION(Alive)		// Macro registering the object into list
    };

}

#endif	/* ALIVE_HPP */

