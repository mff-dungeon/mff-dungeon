#ifndef WEARABLE_HPP
#define	WEARABLE_HPP

#include "../common.hpp"
#include "Item.hpp"
#include "../Actions/MultiTargetAction.hpp"
#include "../ActionDescriptor.hpp"
#include "Room.hpp"

namespace Dungeon {
	
	class Wearable : public Item {
	friend class UnequipAction;
	friend class EquipAction;
	public:
		/**
		 *	Holds the slot for the wearable. Should be grouped logically for easier printing,
		 *  e.g. for(Slot::BodyArmor -> Slot::Helmet): group & print
		 */
		enum Slot {
                    Invalid = 0,
                    Weapon = 1,
                    Backpack = 2,
                    BodyArmor = 3
                };
		/**
		 * Used for actions
		 */
		enum DesiredAction {
			NotKnown = 0,
			Drop = 1,
			Keep = 2
		};
		
		static const char* SlotRelations[];
		
		Wearable() {}
		Wearable(objId id) : Item(id) {}
		virtual ~Wearable() {}
		
		Slot getSlot() const;
		Wearable* setSlot(Slot slot);
		int getAttackBonus() const;
		Wearable* setAttackBonus(int bonus);
		int getDefenseBonus() const;
		Wearable* setDefenseBonus(int bonus);
		
		static bool unequip(ActionDescriptor* ad, ObjectPointer item, Wearable::DesiredAction action = NotKnown);
		
		virtual void getActions(ActionList* list, ObjectPointer callee);
                
		virtual void registerProperties(IPropertyStorage& storage);

		
	private:
		
		Slot slot = Invalid;
		
		int attackBonus = 0;
		int defenseBonus = 0;
		
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
		
		/**
		 * Equips any general not-backpack item
         * @param ad ActionDescriptor
         * @param item the new item to be equiped
         * @param equipedItem currently equiped item, 0 if none is
		 * @param desiredAction desired action for the unequip, 1 if drop, 2 if put to backpack
         */
		void equipItem(ActionDescriptor* ad, ObjectPointer item, ObjectPointer equipedItem = nullptr, int desiredAction = 0);
		
		/**
		 * Provides special treatment to backpacks. Handles item switching, backpack switching, 
		 * weight checking and more.
         * @param ad ActionDescriptor
         * @param newPack The newly equiped backpack
         * @param currentPack The rusty old backpack
         */
		void equipBackpack(ActionDescriptor* ad, ObjectPointer newPack, ObjectPointer currentPack);
	};
	
	class UnequipAction : public MultiTargetAction {
	public:
		UnequipAction(string type = "wearable-unequip") : MultiTargetAction(type) {}
	
		virtual void commit(ActionDescriptor* ad);
		virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);
		virtual void explain(ActionDescriptor* ad);
		virtual bool matchCommand(string command);
	};

}

#endif	/* WEARABLE_HPP */

