#ifndef ALIVE_HPP
#define	ALIVE_HPP

#include "../common.hpp"
#include "../Actions/CallbackAction.hpp"
#include "IDescriptable.hpp"
#include "Wearable.hpp"
#include "Resource.hpp"

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
		/**
		 * State representing current status of Alive
		 * Meanings:
		 *  Living - Alive is ok, can do all the things
		 *  Dying - Used for creatures, will show "finish him" action
		 *  Dead - Used for dead alives eligible to respawn, should now allow any actions except respawn
		 */
		enum State {
			Invalid = 0,
			Living = 1,
			Dying = 2,
			Dead = 3
		};
                
                /**
                 * Describes presence in game.
                 * Meanings:
                 *  Present - Alive has sent a message in last 10 minutes
                 *  Away - There was no interaction for 40 minutes
                 *  Offline - After hour of no interaction
                 */
                enum Presence {
                    Present = 0,
                    Away = 1,
                    Offline = 2
                };
		
        Alive() {}		// Constructor allowing to load class later
        Alive(objId id);
        void getAllActions(ActionList* list);
        virtual void getActions(ActionList* list, ObjectPointer callee);
        
        virtual string getDescriptionSentence();
        virtual string getGroupDescriptionSentence(vector<ObjectPointer> others);
	
		virtual int getCurrentHp() const;
		virtual Alive* setCurrentHp(int hp, ActionDescriptor* ad = 0);
		virtual int getMaxHp() const;
		virtual Alive* setMaxHp(int hp, ActionDescriptor* ad = 0);
		virtual double getPercentageHp();
		virtual int getAttack() const;
		virtual Alive* setAttack(int attack, ActionDescriptor* ad = 0);
		virtual int getDefense() const;
		virtual Alive* setDefense(int defense, ActionDescriptor* ad = 0);
		
		/**
		 * Calls method to damage alive. Calculates the actual damage and calls reduceHp
         * @param amount The attacker's attack value
         */
		virtual Alive* damageAlive(ObjectPointer attackerPtr, int amount, ActionDescriptor* ad = 0);
		
		/**
		 * Adds (or removes) hitpoints. Checks for death or full hitpoints.
         * @param amount The amount to increase (reduce, if negative)
         */
		virtual Alive* changeHp(int amount, ActionDescriptor* ad = 0);
		
		/**
		 * Implements the dying procedures
         */
		virtual Alive* die(ActionDescriptor* ad = 0);
		virtual Alive* respawn(ActionDescriptor* ad = 0);
		
		virtual State getState() const;
		virtual Alive* setState(State newState);
		virtual int getRespawnTime() const;
		virtual Alive* setRespawnTime(int time);
		virtual int getRespawnInterval() const;
		virtual Alive* setRespawnInterval(int interval);
		/**
		 * Counts only on humans, creatures respawn where they were killed.
		 */
		virtual ObjectPointer getRespawnLocation();
		virtual Alive* setRespawnLocation(ObjectPointer room);
		
		/**
		 * Recalculates this alive's attack and defense values due to equip changes
		 * Probably only important for humans, but to avoid casting to Human*
		 * implemented here
         */
		virtual Alive* calculateBonuses();
		
		/**
		 * @return Current location of this being
		 */
		ObjectPointer getLocation();

		/**
		 * @return Equipped backpack or nullptr
		 * 
		 * When there will be more than one backpack, one could implement "IterableObjectPointer",
		 * which utilizes operator++
		 */
		ObjectPointer getBackpack();
		
		/**
		 * Returns true, whether the user has at least one of required item type
         */
		bool hasItemType(string type);

		virtual void registerProperties(IPropertyStorage& storage);

		virtual void onBeforeAction(ActionDescriptor* ad) {
			getLocation()->triggerTraps("inside", ad);
		}

		virtual void onAfterAction(ActionDescriptor* ad) {
			getLocation()->triggerTraps("inside", ad);
		}
                
		int getResourceQuantity(Resource::ResourceType type);
		Alive* setResourceQuantity(Resource::ResourceType type, int quantity);
		Alive* changeResourceQuantity(Resource::ResourceType type, int deltaQuantity);

		bool hasResourceGreaterThan(Resource::ResourceType type, int quantity) {
			int actual = getResourceQuantity(type);
			return actual >= quantity;
		}

		bool hasResourceLowerThan(Resource::ResourceType type, int quantity)  {
			int actual = getResourceQuantity(type);
			return actual <= quantity;
		}

		bool hasResourceEqual(Resource::ResourceType type, int quantity) {
			int actual = getResourceQuantity(type);
			return actual == quantity;
		}    

		Alive* addResource(Resource* resource);
		
		virtual string getWeaponName() const
		{
			return weaponName;
		}

		virtual Alive* setWeaponName(string weaponName)
		{
			this->weaponName = weaponName;
			return this;
		}

		/**
		 * Helper method to set auto-healing trap.
		 * @see Healing
		 */
		virtual Alive* regenerate(int rate);
                
                /**
                 * Finds out whether the guy is online.
                 */
                Presence getPresence();
                
                /**
                 * Use this method to mark every last interaction this guy makes.
                 */
                Alive* markInteraction();
                
	private:
		// Combat stats
        int maxHp = 300;
		int currentHp = 300;
		int defense = 1;
		int attack = 1;
		
		// Respawn related stats
		State currentState = State::Living;
		/**
		 * Timestamp, when it respawns
		 */
		int respawnTime = 0;
		/**
		 * Interval, time it takes after death to respawn, -1 is not respawning
		 */
		int respawnInterval = -1;
                
		string weaponName = "";
                
                /**
                 Presence tracking
                 */
                long lastInteraction = 0;
	
	PERSISTENT_DECLARATION(Alive, IDescriptable)
    };

}

#endif	/* ALIVE_HPP */

