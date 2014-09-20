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
		
		virtual Human* learnSpell(ObjectPointer spell);
		virtual bool knowsSpell(ObjectPointer spell);
		virtual void addCastableSpells(ActionList* list);

		virtual Alive* die(ActionDescriptor* ad = 0);
		virtual Alive* respawn(ActionDescriptor* ad = 0);

		virtual int getCraftingLevel() const;
		virtual Human* addCraftingExp(int exp);
		virtual int getRequiredExp(int level);

        virtual string getName() const;
        virtual string getLongName() const;
        
        virtual void registerProperties(IPropertyStorage& storage);
        
        Human* setContact(string contact);
        string getContact() const;
        Human* setUsername(string username);
        string getUsername() const;
        

        virtual string getWeaponName() const {
            Wearable* weapon = getSingleRelation(Wearable::SlotRelations[Wearable::Slot::Weapon]).safeCast<Wearable>();
            if (weapon)
                return weapon->getName();
            return Alive::getWeaponName();
        }

        
    private:
        string username, contact;
		
		// Level stats
		int craftingLvl = 1;
		int craftingExp = 0;
        
    PERSISTENT_DECLARATION(Human, Alive)

    };
}

#endif	/* HUMAN_HPP */

