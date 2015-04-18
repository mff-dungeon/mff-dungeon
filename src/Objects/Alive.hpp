#ifndef ALIVE_HPP
#define	ALIVE_HPP

#include "../common.hpp"
#include "../Actions/CallbackAction.hpp"
#include "Virtual/IDescriptable.hpp"
#include "Resource.hpp"

namespace Dungeon {

	/**
	 * Basic class for everything alive. Not only humans, but also NPCs.
	 * Basic property of alive is its health and location.
	 * This class should implement "the death".
	 */
	class Alive : public IDescriptable {
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

		Alive() { } // Constructor allowing to load class later
		Alive(const objId& id) : IDescriptable(id) { }

		void getAllActions(ActionDescriptor* list);
		virtual void getActions(ActionDescriptor* list);

		virtual string getDescriptionSentence();
		virtual string getGroupDescriptionSentence(vector<ObjectPointer> others);

		int getCurrentHp() const;
		Alive* setCurrentHp(int hp, ActionDescriptor* ad = nullptr);
		int getMaxHp() const;
		Alive* setMaxHp(int hp, ActionDescriptor* ad = nullptr);
		double getPercentageHp();
		int getAttack() const;
		Alive* setAttack(int attack, ActionDescriptor* ad = nullptr);
		int getDefense() const;
		Alive* setDefense(int defense, ActionDescriptor* ad = nullptr);

		/**
		 * Calculates the actual damage depending on attack/defense values
		 * @param amount The attacker's attack value
		 */
		virtual int calculateDamage(ObjectPointer attackerPtr, int amount);
		/**
		 * Calls method to damage alive. Fills ad and calls reduceHp
		 * @param amount the damage to be done
		 */
		virtual Alive* damageAlive(ObjectPointer attackerPtr, int amount, ActionDescriptor* ad = nullptr);

		/**
		 * Adds (or removes) hitpoints. Checks for death or full hitpoints.
		 * @param amount The amount to increase (reduce, if negative)
		 */
		virtual Alive* changeHp(int amount, ActionDescriptor* ad = nullptr);

		/**
		 * Implements the dying procedures
		 */
		virtual Alive* die(ActionDescriptor* ad = nullptr);
		virtual Alive* respawn(ActionDescriptor* ad = nullptr);

		State getState() const;
		Alive* setState(State newState);
		int getRespawnTime() const;
		Alive* setRespawnTime(int time);
		int getRespawnInterval() const;
		Alive* setRespawnInterval(int interval);
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
		bool hasItemType(const string& type);

		virtual void registerProperties(IPropertyStorage& storage);
		virtual void onBeforeAction(ActionDescriptor* ad) {
			getLocation()->triggerTraps("inside", ad);
		}
		virtual void onAfterAction(ActionDescriptor* ad) {
			getLocation()->triggerTraps("inside", ad);
		}

		virtual string getWeaponName() const {
			return weaponName;
		}
		virtual Alive* setWeaponName(const string& weaponName) {
			this->weaponName = weaponName;
			return this;
		}

		/**
		 * Helper method to set auto-healing trap.
		 * @see Healing
		 */
		Alive* regenerate(int rate);

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

		PERSISTENT_DECLARATION(Alive, IDescriptable)
	};
}

#endif	/* ALIVE_HPP */

