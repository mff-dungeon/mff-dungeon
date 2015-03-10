#ifndef DOORLOCK_HPP
#define	DOORLOCK_HPP

#include "../../common.hpp"
#include "Trap.hpp"

namespace Dungeon {

	/**
	 *	A lock for the door, which checks a user for a key
	 */
	class DoorLock : public Trap {
	public:
		DoorLock() {}
		DoorLock(const objId& id) : Trap(id) {}
		virtual ~DoorLock() {}

		ObjectPointer getKey() const;
		DoorLock* setKey(ObjectPointer key);
		bool consumesKey() const;
		DoorLock* setConsumesKey(bool consumesKey);

		virtual void trigger(const string& event, ObjectPointer target, ActionDescriptor* ad);
		virtual bool exceptionTrigger(ActionDescriptor* ad);

		virtual void registerProperties(IPropertyStorage& storage);

	private:
		string keyId = "";
		bool consumeKey = false;

		PERSISTENT_DECLARATION(DoorLock, Trap)
	};
}

#endif	/* DOORLOCK_HPP */

