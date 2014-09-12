#include "SimpleDamageTrap.hpp"
#include "../ActionDescriptor.hpp"
#include "../GameManager.hpp"

namespace Dungeon {
	
	void SimpleDamageTrap::trigger(string event, ObjectPointer target, ActionDescriptor* ad) {
		if (justOnce && hasRelation("damage-dealt", ad->getAlive()))
			return;
		
		*ad << getDamageMessage() << eos;
		ad->getAlive()->changeHp(-getDamage());
		if (justOnce)
			ad->getGM()->createRelation(this, ad->getAlive(), "damage-dealt");
	}
	
	void SimpleDamageTrap::registerProperties(IPropertyStorage& storage) {
		storage.have(damage, "simpledmgtrap-damage", "Damage dealt")
				.have(damageMessage, "simpledmgtrap-message", "Message on trigger")
				.have(justOnce, "simpledmgtrap-jsutonce", "Trigger just once for each user");
		Trap::registerProperties(storage);
	}
	
	PERSISTENT_IMPLEMENTATION(SimpleDamageTrap)


}

