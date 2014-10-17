#ifndef TELEPORTSPELL_HPP
#define	TELEPORTSPELL_HPP

#include "../common.hpp"
#include "Spell.hpp"

namespace Dungeon {

	class TeleportSpell : public Spell {
	public:
		TeleportSpell();
		TeleportSpell(objId id);
		virtual ~TeleportSpell();

		TeleportSpell* setDestination(ObjectPointer locationPtr);
		ObjectPointer getDestination();

		virtual void cast(ObjectPointer casterPtr, ActionDescriptor* ad);

		PERSISTENT_DECLARATION(TeleportSpell, Spell)
	};
}

#endif	/* TELEPORTSPELL_HPP */