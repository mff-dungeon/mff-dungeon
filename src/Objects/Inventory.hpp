#ifndef INVENTORY_HPP
#define	INVENTORY_HPP

#include "../common.hpp"
#include "../ActionDescriptor.hpp"
#include "../Actions/MultiTargetAction.hpp"
#include "Wearable.hpp"

namespace Dungeon {
	
	class Inventory : public Wearable {
	public:
            const int DEFAULT_SPACE = 20000; // 20 litres
            const int DEFAULT_WEIGHT = 20000; // 20 kg
		Inventory();
		Inventory(objId id);
		virtual ~Inventory();
		
		Inventory* setMaxSpace(int maxSpace);
		int getMaxSpace();
		int getFreeSpace();
		
		Inventory* setMaxWeight(int maxWeight);
		int getMaxWeight();
		int getFreeWeight();
		
		void addItem(Item* item);
		void removeItem(Item* item);
		bool contains(Item* item);
		
		/**
		 * Makes a string describing what is inside
                * 
                 * @return a string for AD
                */
		virtual string getContainingSentence();
		
		virtual void getActions(ActionList* list, ObjectPointer callee);
		
		virtual void registerProperties(IPropertyStorage& storage);
	private:
		int maxSpace;
		int maxWeight;
		int usedSpace;
		int usedWeight;
		
	PERSISTENT_DECLARATION(Inventory, Wearable)
	};
	
	class DropAction : public MultiTargetAction {
	public:
		DropAction(string type = "inventory-drop") : MultiTargetAction(type) {}
        
        virtual void explain(ActionDescriptor* ad);
        virtual bool matchCommand(string command);
        virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);

        virtual void commit(ActionDescriptor* ad);
	};
}

#endif	/* INVENTORY_HPP */

