#ifndef CREATURE_HPP
#define	CREATURE_HPP

#include "../common.hpp"
#include "Alive.hpp"
#include "../Actions/MultiTargetAction.hpp"
#include "../FuzzyStringMatcher.hpp"

namespace Dungeon {
	
	/**
	 * Represents every attackable enemy creature
	 * Registers attack/finish actions
     */
	class Creature : public Alive {
	public:
		Creature() {}
		Creature(objId id) : Alive(id) {};
		virtual ~Creature() {}

		virtual Alive* changeHp(int amount, ActionDescriptor* ad);

		virtual Creature* calculateDrops(ActionDescriptor*ad = 0);
		virtual Creature* attachDrop(ObjectPointer drop);
		virtual Creature* detachDrop(ObjectPointer drop);
		
		virtual Alive* die(ActionDescriptor* ad = 0);
		virtual Alive* respawn(ActionDescriptor* ad = 0);

		virtual void getActions(ActionList* list, ObjectPointer callee);
		
		virtual string getDescriptionSentence();
		virtual string getGroupDescriptionSentence(vector<ObjectPointer> others);

		/**
		 * Helper method to set the creature drops
		 * @see Dropper
		 */
		virtual Creature* drops(ObjectPointer item, int chance, int min = 1, int max = 1);
		
		virtual Creature* setExpReward(int reward);
		virtual int getExpReward() const;
                
	private:
		int expReward = 0;
		
	PERSISTENT_DECLARATION(Creature, Alive)
	};
	
	class CombatAction : public MultiTargetAction {
	public:
		enum CombatMatch {
			Invalid = 0,
			Attack = 1,
			Check = 2,
			Run = 3
		};
		
		CombatAction(string type = "creature-combat") : MultiTargetAction(type) {}
		virtual ~CombatAction(){}

        virtual void explain(ActionDescriptor* ad);
        virtual bool match(string command, ActionDescriptor* ad);
        virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);
		
		bool checkValidity(ActionDescriptor* ad);
		void combatLoop(ActionDescriptor* ad, string reply);
		CombatMatch matchAnswer(string reply);
	private:
		ObjectPointer creaturePtr;
		string text = "\nType 'attack' (a) to attack the enemy, 'check' (c) to check its status or 'run' (r) to run from the fight.";
	};
	
	class KillAction : public MultiTargetAction {
	public:
		KillAction(string type = "creature-kill") : MultiTargetAction(type) {}
        
        virtual void explain(ActionDescriptor* ad);
        virtual bool match(string command, ActionDescriptor* ad);
        virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);
	};
}

#endif	/* CREATURE_HPP */

