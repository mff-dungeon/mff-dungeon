#ifndef HUMAN_HPP
#define	HUMAN_HPP

#include "../common.hpp"
#include "Alive.hpp"

namespace Dungeon {
    
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
        Human(const Human& orig);
        virtual ~Human();
        Human(objId id) : Alive(id) {}
        Human(objId id, string username, string contact) :
        Alive(id), username(username), contact(contact) {}

        virtual string getName() const;
        virtual string getLongName() const;
        
        virtual void serialize(Archiver& stream);
        
        Human* setContact(string contact);
        string getContact() const;
        Human* setUsername(string username);
        string getUsername() const;


    private:
        string username, contact;
        
    PERSISTENT_DECLARATION(Human)

    };
}

#endif	/* HUMAN_HPP */

