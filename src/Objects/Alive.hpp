#ifndef ALIVE_HPP
#define	ALIVE_HPP

#include "../common.hpp"
#include "../Actions/CallbackAction.hpp"
#include "IDescriptable.hpp"


namespace Dungeon
{

    /**
     * Basic class for everything alive. Not only humans, but also NPCs.
     * Basic property of alive is its health and location.
     * This class should implement "the death".
     */
    class Alive : public IDescriptable
    {
    public:
        Alive() {}		// Constructor allowing to load class later
        Alive(objId id);
        void getAllActions(ActionList* list);
        virtual void getActions(ActionList* list, IObject *callee);
        int hitpoints;
        
        virtual string getDescriptionSentence();
        virtual string getGroupDescriptionSentence(vector<IDescriptable *> others);
	
        /**
         * @return Current location of this being
         */
        ObjectPointer getLocation();
        
        virtual void registerProperties(IPropertyStorage& storage);
	
	PERSISTENT_DECLARATION(Alive, IDescriptable)
    };

}

#endif	/* ALIVE_HPP */

