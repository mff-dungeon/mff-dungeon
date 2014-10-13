#ifndef ITEMRESPAWNTRAP_HPP
#define ITEMRESPAWNTRAP_HPP

#include "../common.hpp"
#include "Trap.hpp"

namespace Dungeon {

	class ItemRespawner : public Trap {
	public:
		ItemRespawner() : Trap() { }
		ItemRespawner(string id) : Trap(id) { }
		virtual ~ItemRespawner() { }

		enum RespawnMode {
			/* No matter what happens next, the item will respawn <interval> seconds after picking*/
			AfterPick = 0,
			/* Item respawns after continuous <interval> seconds not being there*/
			AfterDisappear = 1
		};

		/**
		 * Special values for interval
		 */
		enum Timestamp : int {
			Now = 0,
			Auto = -1,
			NotNow = -2,
		};

		ObjectPointer setTemplate(ObjectPointer templ);
		ObjectPointer getTemplate();
		int getInterval() const {
			return interval;
		}
		/**
		 * Respawn interval in seconds
		 * @param interval
		 */
		ObjectPointer setInterval(int interval) {
			this->interval = interval;
			return this;
		}
		int getTimestamp() const {
			return timestamp;
		}
		ObjectPointer setTimestamp(int timestamp = Timestamp::Auto) {
			if (timestamp == Timestamp::Auto) {
				timestamp = std::time(0) + getInterval();
			}
			this->timestamp = timestamp;
			return this;
		}

		virtual void trigger(string event, ObjectPointer location, ActionDescriptor* ad);
		virtual ObjectPointer doRespawn(ObjectPointer location);

		virtual void registerProperties(IPropertyStorage& storage);

		PERSISTENT_DECLARATION(ItemRespawner, Trap);
	private:
		int interval = 60, timestamp = Timestamp::Now;
		RespawnMode mode = RespawnMode::AfterPick;
	};
}

#endif

