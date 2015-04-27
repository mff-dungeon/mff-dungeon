#ifndef CREATURE_HPP
#define	CREATURE_HPP

#include "../common.hpp"
#include "../Utils/FuzzyStringMatcher.hpp"
#include "../Actions/MultiTargetAction.hpp"
#include "Alive.hpp"

namespace Dungeon {

	/**
	 * Represents every attackable enemy creature
	 * Registers attack/finish actions
	 */
	class Creature : public Alive {
	public:
		Creature() { }
		Creature(const objId& id) : Alive(id) { };
		virtual ~Creature() { }

		virtual Alive* changeHp(int amount, ActionDescriptor* ad);

		virtual Creature* calculateDrops(ActionDescriptor* ad = nullptr);
		Creature* attachDrop(ObjectPointer drop);
		Creature* detachDrop(ObjectPointer drop);

		virtual Alive* die(ActionDescriptor* ad = nullptr);
		virtual Alive* respawn(ActionDescriptor* ad = nullptr);

		virtual void getActions(ActionDescriptor* ad);

		virtual void examine(ActionDescriptor* ad);
		virtual string getDescriptionSentence();
		virtual string getGroupDescriptionSentence(vector<ObjectPointer> others);

		/**
		 * Helper method to set the creature drops
		 * @see Dropper
		 */
		Creature* drops(ObjectPointer item, int chance, int min = 1, int max = 1);

		Creature* setExpReward(int reward);
		int getExpReward() const;

	private:
		int expReward = 0;

		PERSISTENT_DECLARATION(Creature, Alive)
	};

	class CombatAction : public MultiTargetAction {
	public:

		enum CombatMatch {
			Ask = 0,
			Attack = 1,
			Check = 2,
			Run = 3,
			Help = 4,
			Berserk = 5
		} combatState = Ask;

		CombatAction(const string& type = "creature-combat") : MultiTargetAction(type) {
                    messages.unspecified = "You can attack either %. Which one first?";
                    messages.nextTime = "You should tell me what do you want to attack. But this time it was pretty obvious.";
                    messages.dontCare = "If you don't care, I'll just attack %.";
                    messages.uncertain = "Do you want to strike %?";
                    messages.noCandidate = "There's no enemy of that name. Please look around and try it again.";
                    messages.itRegex = "it|there";
                    messages.anyRegex = "(any|some|anything)(\\s+(.*))?;";
                    messages.anyRegexMatchIndex = 3;
                }
		virtual ~CombatAction() { }

		virtual void explain(ActionDescriptor* ad);
		virtual bool match(const string& command, ActionDescriptor* ad);
		virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);

		bool checkValidity(ActionDescriptor* ad);
		void combatLoop(ActionDescriptor* ad, const string& reply);
		void combatStep(ActionDescriptor* ad, CombatMatch action);
		CombatMatch matchAnswer(const string& reply);
	private:
		ObjectPointer creaturePtr;
		bool combatModeInformed = false;

	};

        /**
         * @deprecated
         */
	class KillAction : public MultiTargetAction {
	public:
		KillAction(const string& type = "creature-kill") : MultiTargetAction(type) { }

		virtual void explain(ActionDescriptor* ad);
		virtual bool match(const string& command, ActionDescriptor* ad);
		virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);
	};
}

#endif	/* CREATURE_HPP */

