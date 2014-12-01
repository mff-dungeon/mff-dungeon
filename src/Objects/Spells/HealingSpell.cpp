#include "HealingSpell.hpp"
#include "../Human.hpp"

namespace Dungeon {

	void HealingSpell::cast(ObjectPointer casterPtr, ActionDescriptor* ad) {
		Human* caster = casterPtr.assertExists("The caster doesn't exist")
				.assertType<Human>("Only humans cast spells")
				.unsafeCast<Human>();
		caster->changeResourceQuantity(Resource::ManaShard, -(getManaCost(casterPtr)));
		*ad << "You have cast " << this->getName() << "." << eos;
		caster->changeHp(getEffect(casterPtr), ad);
	}
	
	PERSISTENT_IMPLEMENTATION(HealingSpell)
}