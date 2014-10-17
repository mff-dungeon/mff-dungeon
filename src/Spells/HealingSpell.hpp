#ifndef HEALINGSPELL_HPP
#define	HEALINGSPELL_HPP

#include "../common.hpp"
#include "Spell.hpp"

namespace Dungeon {

	class HealingSpell : public Spell {
	public:
		HealingSpell();
		HealingSpell(objId id);
		virtual ~HealingSpell();

		virtual void cast(ObjectPointer casterPtr, ActionDescriptor* ad = 0);
		
		PERSISTENT_DECLARATION(HealingSpell, Spell)
	};
}
#endif	/* HEALINGSPELL_HPP */

