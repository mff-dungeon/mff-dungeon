#ifndef HUMAN_HPP
#define	HUMAN_HPP

#include "../common.hpp"
#include "Alive.hpp"

namespace Dungeon {
    
	/**
	 * Default respawn interval in seconds for humans, testing value, could be changed later (or used kinda dynamically)	 
	 */
	#define DEFAULT_RESPAWN_INTERVAL 60

    /**
     * A player's representation in Dung world.
     * Bears only contact information.
     * 
     * Can be overriden with another "human", this one 
     * is to be driven by jabber.
     */
    class Human : public Alive {
    public:
        Human();
        virtual ~Human();
        Human(objId id);
        Human(objId id, string username, string contact);
		
        virtual void getActions(ActionList* list, ObjectPointer callee);

		virtual Alive* die(ActionDescriptor* ad = 0);
		virtual Alive* respawn(ActionDescriptor* ad = 0);


        virtual string getName() const;
        virtual string getLongName() const;
        
        virtual void registerProperties(IPropertyStorage& storage);
        
        Human* setContact(string contact);
        string getContact() const;
        Human* setUsername(string username);
        string getUsername() const;
		
    private:
        string username, contact;
        
    PERSISTENT_DECLARATION(Human, Alive)

    };
}

#endif	/* HUMAN_HPP */

