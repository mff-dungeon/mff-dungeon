#ifndef HEALING_HPP
#define	HEALING_HPP

#include "../common.hpp"
#include "Trap.hpp"

namespace Dungeon {

	class Healing : public Trap {
	public:
		Healing() : Trap() { }
		Healing(string id) : Trap(id) { }
		virtual ~Healing() { }

		/** Use this to scale while creating the world */
		static const int PerHour = 1;
		static const int PerMin = 60 * PerHour;
		static const int PerSec = 60 * PerMin;

		Healing* setTarget(ObjectPointer target);
		ObjectPointer getTarget() {
			return getSingleRelation(R_TARGET);
		}
		int getRate() const {
			return rate;
		}
		Healing* setRate(int rate) {
			this->rate = rate;
			return this;
		}

		virtual void trigger(string event, ObjectPointer target, ActionDescriptor* ad);

		virtual void registerProperties(IPropertyStorage& storage);

		PERSISTENT_DECLARATION(Healing, Trap)
	private:
		int rate = 1;
		int timestamp = 0;

	};
}

#endif

