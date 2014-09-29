#ifndef HUMAN_HPP
#define	HUMAN_HPP

#include "../common.hpp"
#include "Alive.hpp"

namespace Dungeon {
    
	/**
	 * Default respawn interval in seconds for humans, testing value, could be changed later (or used kinda dynamically)	 
	 */
	#define DEFAULT_RESPAWN_INTERVAL 120

	/**
	 * The number of points which the user will receive on level up, 
	 * can be distributed to the stats
	 */
	#define LEVEL_STAT_POINTS 5

    /**
     * A player's representation in Dung world.
     * Bears only contact information.
     * 
     * Can be overriden with another "human", this one 
     * is to be driven by jabber.
     */
    class Human : public Alive {
    public:
		enum Stats {
			Begin = 0,
			Strength = Begin,	// attacking
			Dexterity = 1,	// defending
			Vitality = 2,	// hps
			Intelligence = 3, // spell strength
			Wisdom = 4,		// spell cost
			Crafting = 5,	// crafting skills
			Alchemy = 6,		// potion skills
			End = 7
		};
		
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

		/**
		 * @deprecated
         */
		virtual int getCraftingLevel() const;
		/**
		 * @deprecated
         */
		virtual Human* addCraftingExp(int exp);
		
		virtual int getCharacterLevel() const;
		virtual int getFreePoints() const;
		virtual int getStatValue(Stats stat) const;
		virtual int getStatValue(int stat) const {
			return getStatValue(Stats(stat));
		}
		virtual Human* changeStatValue(Stats stat, int delta, ActionDescriptor* ad = 0);
		virtual Human* setStatValue(Stats stat, int value, ActionDescriptor* ad = 0);
		virtual Human* addExperience(int exp, ActionDescriptor* ad = 0);
		
		/**
		 * Returns a string description of that stat
         * @param stat	Stat to be returned
         * @param pure	If true, returns a lowercase string without whitespace
         */
		static string getStatName(Stats stat, bool pure = false);
		static string getStatName(int stat, bool pure = false) {
			return getStatName(Stats(stat), pure);
		};

        virtual string getName() const;
        virtual string getLongName() const;
        
        virtual void registerProperties(IPropertyStorage& storage);
        
        Human* setContact(string contact);
        string getContact() const;
        Human* setUsername(string username);
        string getUsername() const;
        
        virtual string getWeaponName() const;

        
    private:
        string username, contact;
		
		// Level stats
		// @deprecated
		int craftingLvl = 1;
		// @deprecated
		int craftingExp = 0;
		
		// Character level
		int level = 1;
		// Character experience points
		long exp = 0;
		// Free points to be distributed to stats
		int freepoints = 0;
		int* stats;
		
		/**
		 * Calculates the exp needed to level up
         * @param level level to be reached
         * @return exp required for parameter level
         */
		int getRequiredExp(int level);
		
		/**
		 * Performs a level up
         * @param ad AD to nofity of level up
         */
		void doLevelUp(ActionDescriptor* ad = 0);
		
		/**
		 * Returns a time rounded to the highest order
         */
		string getRoundedTime(int seconds);
        
    PERSISTENT_DECLARATION(Human, Alive)

    };
}

#endif	/* HUMAN_HPP */