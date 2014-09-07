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

		virtual Alive* die(ActionDescriptor* ad = 0);
		virtual Alive* respawn(ActionDescriptor* ad = 0);

		virtual void getActions(ActionList* list, ObjectPointer callee);
		
		virtual string getDescriptionSentence();
		virtual string getGroupDescriptionSentence(vector<ObjectPointer> others);

	private:
		
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
        
        virtual void explain(ActionDescriptor* ad);
        virtual bool matchCommand(string command);
        virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);

        virtual void commit(ActionDescriptor* ad);
		
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
        virtual bool matchCommand(string command);
        virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);

        virtual void commit(ActionDescriptor* ad);
	};
}

#endif	/* CREATURE_HPP */

