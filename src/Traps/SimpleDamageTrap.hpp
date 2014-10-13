#ifndef SIMPLEDAMAGETRAP_HPP
#define	SIMPLEDAMAGETRAP_HPP

#include "../common.hpp"
#include "Trap.hpp"

namespace Dungeon {

	class SimpleDamageTrap : public Trap {
	public:
		SimpleDamageTrap() : Trap() { }
		SimpleDamageTrap(string id) : Trap(id) { }
		virtual ~SimpleDamageTrap() { }
		string getDamageMessage() const {
			return damageMessage;
		}
		SimpleDamageTrap* setDamageMessage(string damageMessage) {
			this->damageMessage = damageMessage;
			return this;
		}
		bool isJustOnce() const {
			return justOnce;
		}
		SimpleDamageTrap* setJustOnce(bool justOnce = true) {
			this->justOnce = justOnce;
			return this;
		}
		int getDamage() const {
			return damage;
		}
		SimpleDamageTrap* setDamage(int damage) {
			this->damage = damage;
			return this;
		}

		virtual void trigger(string event, ObjectPointer target, ActionDescriptor* ad);
		virtual void registerProperties(IPropertyStorage& storage);

	private:
		string damageMessage = "";
		bool justOnce = false;

		int damage = 0;

		PERSISTENT_DECLARATION(SimpleDamageTrap, Trap)
	};
}

#endif

