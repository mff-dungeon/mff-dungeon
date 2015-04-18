#include "HealingSpell.hpp"
#include "../Human.hpp"
#include "../Inventory.hpp"

namespace Dungeon {

	void HealingSpell::cast(ObjectPointer casterPtr, ActionDescriptor* ad) {
		Human* caster = casterPtr.assertExists("The caster doesn't exist")
				.assertType<Human>("Only humans cast spells")
				.unsafeCast<Human>();
		caster->getBackpack().unsafeCast<Inventory>()->removeResource(Resource::ManaShard, getManaCost(casterPtr));
		*ad << "You have cast " << this->getName() << "." << eos;
		caster->changeHp(getEffect(casterPtr), ad);
	}

	PERSISTENT_IMPLEMENTATION(HealingSpell)
}