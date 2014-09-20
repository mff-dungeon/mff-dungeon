#ifndef SPELLSCROLL_HPP
#define	SPELLSCROLL_HPP

#include "../common.hpp"
#include "Item.hpp"
#include "../Actions/MultiTargetAction.hpp"

namespace Dungeon {
	/**
	 *	A scroll for learning a new spell, it is destroyed on use
	 */
	class SpellScroll : public Item {
	public:
		SpellScroll();
		SpellScroll(objId id);
		virtual ~SpellScroll();
		
		/**
		 * Sets the spell for this scroll, which will be learned when read
		 * @param spell spell to be learned
		 */
		SpellScroll* setSpell(ObjectPointer spell);
		ObjectPointer getSpell() const;
		
		virtual void getActions(ActionList* list, ObjectPointer callee);

	private:

	PERSISTENT_DECLARATION(SpellScroll, Item)
	};
	
	// FIXME: Later, read action will be PolyAction (with signs, markers, ...)
	class ReadScrollAction : public MultiTargetAction {
	public:
		ReadScrollAction(string type = "spellscroll-read") : MultiTargetAction(type) {};
        
        virtual void explain(ActionDescriptor* ad);
        virtual bool match(string command, ActionDescriptor* ad);
        virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);
	};
}

#endif	/* SPELLSCROLL_HPP */

