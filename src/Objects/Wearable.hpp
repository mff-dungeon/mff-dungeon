#ifndef WEARABLE_HPP
#define	WEARABLE_HPP

#include "../common.hpp"
#include "Item.hpp"
#include "../Actions/MultiTargetAction.hpp"
#include "../ActionDescriptor.hpp"
#include "Location.hpp"

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
					Shield = 3,
					Helmet = 4,
					Boots = 5,
					Gloves = 6,
                    BodyArmor = 7
                };
		/**
		 * Used for actions
		 */
		enum DesiredAction {
			NotKnown = 0,
			Drop = 1,
			Keep = 2,
			MoveAndKeep = 3,
			MoveAndDrop = 4
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
		int getHpBonus() const;
		Wearable* setHpBonus(int bonus);

		string getEquippedSentence() const;
		virtual string getDescription() const;

		virtual void getActions(ActionList* list, ObjectPointer callee);
                
		virtual void registerProperties(IPropertyStorage& storage);
          
		static bool unequip(ActionDescriptor* ad, ObjectPointer item, Wearable::DesiredAction action = NotKnown);      
		virtual ObjectPointer onEquip(ActionDescriptor* ad) {
			triggerTraps("equipped", ad);
			return this;
		}

		virtual ObjectPointer onUnequip(ActionDescriptor* ad) {
			triggerTraps("unequipped", ad);
			return this;
		}
                	
	private:
		
		Slot slot = Invalid;
		
		int attackBonus = 0;
		int defenseBonus = 0;
		int hpBonus = 0;
		
	PERSISTENT_DECLARATION(Wearable, Item)
	};
	
	
	class UnequipAction : public MultiTargetAction {
	public:
		UnequipAction(string type = "wearable-unequip") : MultiTargetAction(type) {}
	
		virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);
		virtual void explain(ActionDescriptor* ad);
		virtual bool match(string command, ActionDescriptor* ad);
	};

	/**
	 * Action handling item equipping. The action itself is quite complex, because we need to find the item first.
     */
	class EquipAction : public MultiTargetAction {
	private:
		Wearable::DesiredAction dAction = Wearable::DesiredAction::NotKnown;
		ObjectPointer itemPtr;
		ObjectPointer equipedItemPtr;
                Wearable::Slot slot;
                string slotRelation;
	public:
		EquipAction(string type = "wearable-equip") : MultiTargetAction(type) {}
		
		virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);
		virtual void explain(ActionDescriptor* ad);
		virtual bool match(string command, ActionDescriptor* ad);
		
		/**
		 * Phase one handles asking user question and matching the answer
         */
		void itemPhaseOne(ActionDescriptor* ad);
		/**
		 * Phase two of item equip. It is called when the user currently has something on.
		 * It is handed a DesiredAction, what is supposed to be done
         */
		void itemPhaseTwo(ActionDescriptor* ad);
		/**
		 * Phase three is called when there is nothing equipped in the slot. Handles equipping.
         */
		void itemPhaseThree(ActionDescriptor* ad);
		
		/**
		 * Phase one handles reply matching
         */
		void backpackPhaseOne(ActionDescriptor* ad);
		/**
		 * Phase two handles handling the old backpack (dropping / moving the items)
         */
		void backpackPhaseTwo(ActionDescriptor* ad);
		/**
		 * Phase three finished the action by updating room, adding backpack and changing stats
         */
		void backpackPhaseThree(ActionDescriptor* ad);
	};
}

#endif	/* WEARABLE_HPP */

