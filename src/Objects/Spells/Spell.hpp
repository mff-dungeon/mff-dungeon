#ifndef SPELL_HPP
#define	SPELL_HPP

#include "../../common.hpp"
#include "../Virtual/IDescriptable.hpp"
#include "../../Actions/MultiTargetAction.hpp"
#include "../../Game/ActionDescriptor.hpp"

namespace Dungeon {

	/**
	 *	A basic abstract class for all spells
	 */
	class Spell : public IDescriptable {
	public:
		Spell() {};
		Spell(const objId& id) : IDescriptable(id) {}
		virtual ~Spell() {}

		int getBaseManaCost() const;
		Spell* setBaseManaCost(int mana);
		int getBaseEffect() const;
		Spell* setBaseEffect(int effect);
		int getBaseInt() const;
		Spell* setBaseInt(int baseInt);
		int getBaseWis() const;
		Spell* setBaseWis(int baseWis);

		/**
		 * Calculates mana cost depending on caster's stats
		 * @param casterPtr User casting this spell
		 * @return mana cost of the spell
		 */
		int getManaCost(ObjectPointer casterPtr) const;
		/**
		 * Calculates effect value depending on caster's stats
		 * @param casterPtr User casting this spell
		 * @return effect strength of the spell
		 */
		int getEffect(ObjectPointer casterPtr) const;

		/**
		 * Checks if the spell can be cast. Checks for mana, required stats, 
		 * and other validity
		 * @param casterPtr User casting this spell
		 * @param ad An AD to message the user, if desired
		 * @return true if the spell can be casted
		 */
		virtual bool checkCast(ObjectPointer casterPtr, ActionDescriptor* ad = nullptr) const;
		/**
		 * Does the actual spell effect, to be overriden
		 * @param casterPtr User casting this spell
		 * @param ad An AD to message the user, if desired
		 */
		virtual void cast(ObjectPointer casterPtr, ActionDescriptor* ad = nullptr);

		virtual void registerProperties(IPropertyStorage& storage);

	private:
		int manaCost = 0;
		int effect = 0;
		int baseInt = 1; // if user's int == spell's int, effect = baseEffect
		int baseWis = 1; // similar to int with manaCost

		PERSISTENT_DECLARATION(Spell, IDescriptable)
	};

	class CastAction : public MultiTargetAction {
	public:
		CastAction(string type = "spell-cast") : MultiTargetAction(type) { };

		virtual void explain(ActionDescriptor* ad);
		virtual bool match(const string& command, ActionDescriptor* ad);
		virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);
	};
}

#endif	/* SPELL_ */

