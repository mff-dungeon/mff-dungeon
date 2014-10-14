#ifndef HUMAN_HPP
#define	HUMAN_HPP

#include "../common.hpp"
#include "../IObject.hpp"
#include "Alive.hpp"
#include "../Action.hpp"

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
		virtual ~Human();
		Human(objId id);
		Human(objId id, string username, string contact);

		virtual void getActions(ActionList* list, ObjectPointer callee);

		virtual Human* learnSpell(ObjectPointer spell);
		virtual bool knowsSpell(ObjectPointer spell);
		virtual void addCastableSpells(ActionList* list);

		virtual Alive* die(ActionDescriptor* ad = 0);
		virtual Alive* respawn(ActionDescriptor* ad = 0);

		virtual int getCharacterLevel() const;
		virtual int getExperience() const;
		virtual int getFreePoints() const;
		virtual Human* useStatPoint(Stats stat, ActionDescriptor* ad = 0);
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
		virtual Alive* calculateBonuses();

		virtual string getName() const;
		virtual string getLongName() const;

		virtual void registerProperties(IPropertyStorage& storage);

		Human* setContact(string contact);
		string getContact() const;
		Human* setUsername(string username);
		string getUsername() const;

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

		/**
		 Presence tracking
		 */
		long lastInteraction = 0;

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

	class StatReq : public IObject {
	public:
		StatReq() { };
		StatReq(objId id) : IObject(id) { };
		~StatReq() { };
		StatReq(Human::Stats stat, int value) {
			this->stat = stat;
			this->value = value;
		};
		virtual void getActions(ActionList * list, ObjectPointer callee) { };
		int getValue() const;
		StatReq* setValue(int value);
		Human::Stats getStat() const;
		StatReq* setStat(Human::Stats stat);

		virtual void registerProperties(IPropertyStorage& storage);
	private:
		int value;
		Human::Stats stat;

		PERSISTENT_DECLARATION(StatReq, IObject);
	};

	class RaiseStatAction : public Action {
	public:
		RaiseStatAction(string type = "raise-stat") : Action(type) { };

		void selectStat(string statName, ActionDescriptor* ad);

		virtual bool match(string command, ActionDescriptor* ad);
		virtual void commit(ActionDescriptor* ad);
		virtual void explain(ActionDescriptor* ad);
	private:
		Human::Stats selectedStat = Human::End;
	};
}

#endif	/* HUMAN_HPP */