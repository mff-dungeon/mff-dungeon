#ifndef SPELL_HPP
#define	SPELL_HPP

#include "../common.hpp"
#include "IDescriptable.hpp"
#include "../Actions/MultiTargetAction.hpp"
#include "../ActionDescriptor.hpp"

namespace Dungeon {

	/**
	 *	A basic abstract class for all spells
	 */
	class Spell : public IDescriptable {
	public:
		Spell();
		Spell(objId id);
		virtual ~Spell();
		
		int getBaseManaCost() const;
		Spell* setBaseManaCost(int mana);
		int getBaseEffect() const;
		Spell* setBaseEffect(int effect);
		
		/**
		 * Calculates mana cost depending on caster's stats
         * @param casterPtr User casting this spell
         * @return mana cost of the spell
         */
		int getManaCost(ObjectPointer casterPtr) const;
		/**
		 * Calculates effect value depending on caster's stats
         * @param casterPtr User casting this spell
         * @return mana cost of the spell
         */
		int getEffect(ObjectPointer casterPtr) const;
		
		/**
		 * Checks if the spell can be cast. Checks for mana, required stats, 
		 * and other validity
         * @param casterPtr User casting this spell
         * @param ad An AD to message the user, if desired
         * @return true if the spell can be casted
         */
		virtual bool checkCast(ObjectPointer casterPtr, ActionDescriptor* ad = 0) const;
		/**
		 * Does the actual spell effect, to be overriden
         * @param casterPtr User casting this spell
         * @param ad An AD to message the user, if desired
         */
		virtual void cast(ObjectPointer casterPtr, ActionDescriptor* ad = 0);
		
		virtual void registerProperties(IPropertyStorage& storage);
		
	private:
		int manaCost = 0;
		int effect = 0;
		// TODO: add some requirements, like high intelligence
	PERSISTENT_DECLARATION(Spell, IDescriptable)
	};
	
	class CastAction : public MultiTargetAction {
	public:
		CastAction(string type = "spell-cast") : MultiTargetAction(type) {};
        
        virtual void explain(ActionDescriptor* ad);
        virtual bool match(string command, ActionDescriptor* ad);
        virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);
	};
}

#endif	/* SPELL_ */

