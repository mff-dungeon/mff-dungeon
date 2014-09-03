#ifndef WEARABLE_HPP
#define	WEARABLE_HPP

#include "../common.hpp"
#include "Item.hpp"
#include "../Actions/MultiTargetAction.hpp"
#include "../ActionDescriptor.hpp"
#include "Room.hpp"

namespace Dungeon {
	
	class Wearable : public Item {
	public:
		enum Slot {
                    Invalid = 0,
                    Backpack = 1,
                    Weapon = 2,
                    BodyArmor = 3
                };
		static const char* SlotRelations[];
		
		Wearable();
		Wearable(objId id);
		virtual ~Wearable();
		
		Slot getSlot() const;
		Wearable* setSlot(Slot slot);
		
		virtual void getActions(ActionList* list, IObject* callee);
                
                virtual void registerProperties(IPropertyStorage& storage);

		
	private:
		Slot slot = Invalid;
		
	PERSISTENT_DECLARATION(Wearable, Item)
	};
	
	/**
	 * Action handling item equipping. The action itself is quite complex, because we need to find the item first.
     */
	class EquipAction : public MultiTargetAction {
	public:
		EquipAction(string type = "wearable-equip") : MultiTargetAction(type) {}
		
		virtual void commit(ActionDescriptor* ad);
		virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);
		virtual void explain(ActionDescriptor* ad);
		virtual bool matchCommand(string command);
		
		// TODO: Any idea how to now use same code twice? make the other static?*
		bool unequip(ActionDescriptor* ad, Wearable* item);
	};
	
	class UnequipAction : public MultiTargetAction {
	public:
		UnequipAction(string type = "wearable-unequip") : MultiTargetAction(type) {}
	
		virtual void commit(ActionDescriptor* ad);
		virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);
		virtual void explain(ActionDescriptor* ad);
		virtual bool matchCommand(string command);
		
		bool unequip(ActionDescriptor* ad, Wearable* item);
	};

}

#endif	/* WEARABLE_HPP */

