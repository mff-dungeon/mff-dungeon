#include "SimpleDamageTrap.hpp"
#include "../ActionDescriptor.hpp"
#include "../GameManager.hpp"

namespace Dungeon {

	void SimpleDamageTrap::trigger(string event, ObjectPointer target, ActionDescriptor* ad) {
		if (justOnce && hasRelation("damage-dealt", ad->getCaller()))
			return;

		*ad << getDamageMessage() << eos;
		ad->getCaller()->changeHp(-getDamage());
		if (justOnce)
			ad->getGM()->createRelation(this, ad->getCaller(), "damage-dealt");
	}

	void SimpleDamageTrap::registerProperties(IPropertyStorage& storage) {
		storage.have(damage, "simpledmgtrap-damage", "Damage dealt")
				.have(damageMessage, "simpledmgtrap-message", "Message on trigger")
				.have(justOnce, "simpledmgtrap-jsutonce", "Trigger just once for each user");
		Trap::registerProperties(storage);
	}

	PERSISTENT_IMPLEMENTATION(SimpleDamageTrap)


}

