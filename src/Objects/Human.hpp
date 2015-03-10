#ifndef HUMAN_HPP
#define	HUMAN_HPP

#include "../common.hpp"
#include "Alive.hpp"
#include "../Actions/Action.hpp"

/**
 * Default respawn interval in seconds for humans, testing value, could be changed later (or used kinda dynamically)	 
 */
#define DEFAULT_RESPAWN_INTERVAL 120

/**
 * The number of points which the user will receive on level up, 
 * can be distributed to the stats
 */
#define LEVEL_STAT_POINTS 5

namespace Dungeon {

	/**
	 * A player's representation in Dung world.
	 * 
	 * Can be overriden with another "human", this one 
	 * is to be driven by jabber.
	 */
	class Human : public Alive {
	public:

		enum Stats {
			Begin = 0,
			Strength = Begin, // attacking
			Dexterity = 1, // defending
			Vitality = 2, // hps
			Intelligence = 3, // spell strength
			Wisdom = 4, // spell cost
			Crafting = 5, // crafting skills
			Alchemy = 6, // potion skills
			End = 7
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

		Human();
		virtual ~Human() { }
		Human(const objId& id);
		Human(const objId& id, const string& username, const string& contact);

		virtual void getActions(ActionList* list, ObjectPointer callee);

		Human* learnSpell(ObjectPointer spell);
		bool knowsSpell(ObjectPointer spell);
		void addCastableSpells(ActionList* list);

		virtual Alive* die(ActionDescriptor* ad = nullptr);
		virtual Alive* respawn(ActionDescriptor* ad = nullptr);

		int getCharacterLevel() const;
		int getExperience() const;
		int getFreePoints() const;
		Human* useStatPoint(Stats stat, ActionDescriptor* ad = nullptr);
		int getStatValue(Stats stat) const;
		int getStatValue(int stat) const {
			return getStatValue(Stats(stat));
		}
		Human* changeStatValue(Stats stat, int delta, ActionDescriptor* ad = nullptr);
		Human* setStatValue(Stats stat, int value, ActionDescriptor* ad = nullptr);
		Human* addExperience(int exp, ActionDescriptor* ad = nullptr);

		/**
		 * Returns a string description of that stat
		 * @param stat	Stat to be returned
		 * @param pure	If true, returns a lowercase string without whitespace
		 */
		static string getStatName(Stats stat, bool pure = false);
		static string getStatName(int stat, bool pure = false) {
			return getStatName(Stats(stat), pure);
		};
		virtual Alive* calculateBonuses();

		virtual string getName() const;
		virtual string getLongName() const;

		virtual void registerProperties(IPropertyStorage& storage);

		Human* setContact(const string& contact);
		const string& getContact() const;
		Human* setUsername(const string& username);
		const string& getUsername() const;

		/**
		 * Finds out whether the guy is online.
		 */
		Presence getPresence();

		/**
		 * Use this method to mark every last interaction this guy makes.
		 */
		Human* markInteraction();

		virtual string getWeaponName() const;


	private:
		string username, contact;

		/**	 Presence tracking */
		long lastInteraction = 0;

		// Level stats
		// Character level
		int level = 1;
		// Character experience points
		long exp = 0;
		// Free points to be distributed to stats
		int freepoints = 0;
		int stats [Stats::End];

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
