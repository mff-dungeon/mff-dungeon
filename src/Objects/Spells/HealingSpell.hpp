#ifndef HEALINGSPELL_HPP
#define	HEALINGSPELL_HPP

#include "../../common.hpp"
#include "Spell.hpp"

namespace Dungeon {

	class HealingSpell : public Spell {
	public:
		HealingSpell() {}
		HealingSpell(const objId& id) : Spell(id) {}
		virtual ~HealingSpell() {}

		virtual void cast(ObjectPointer casterPtr, ActionDescriptor* ad = nullptr);

		PERSISTENT_DECLARATION(HealingSpell, Spell)
	};
}
#endif	/* HEALINGSPELL_HPP */

