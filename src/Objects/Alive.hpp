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
        
        virtual string getDescriptionSentence();
        virtual string getGroupDescriptionSentence(vector<IDescriptable *> others);
	
		virtual int getCurrentHp() const;
		virtual Alive* setCurrentHp(int hp);
		virtual int getMaxHp() const;
		virtual Alive* setMaxHp(int hp);
		virtual int getAttack() const;
		virtual Alive* setAttack(int attack);
		virtual int getDefense() const;
		virtual Alive* setDefense(int defense);
		
		/**
		 * Calls method to damage alive. Calculates the actual damage and calls reduceHp
         * @param amount The attacker's attack value
         */
		virtual Alive* damageAlive(int amount);
		
		/**
		 * Adds (or removes) hitpoints. Checks for death or full hitpoints.
         * @param amount The amount to increase (reduce, if negative)
         */
		virtual Alive* changeHp(int amount);
		
		/**
		 * Implements the dying procedures
         */
		virtual Alive* die();
		
        /**
         * @return Current location of this being
         */
        ObjectPointer getLocation();
        
        virtual void registerProperties(IPropertyStorage& storage);
		
	private:
		/*
		 *	Defaults for persistence
		 */
        int maxHp = 1000;
		int currentHp = 1000;
		int defense = 1;
		int attack = 1;
	
	PERSISTENT_DECLARATION(Alive, IDescriptable)
    };

}

#endif	/* ALIVE_HPP */

