#ifndef INVENTORY_HPP
#define	INVENTORY_HPP

#include "../common.hpp"
#include "../Game/ActionDescriptor.hpp"
#include "../Actions/MultiTargetAction.hpp"
#include "Wearable.hpp"

namespace Dungeon {

	class Inventory : public Wearable {
	public:
		const int DEFAULT_SPACE = 20000; // 20 litres
		const int DEFAULT_WEIGHT = 20000; // 20 kg
		Inventory();
		Inventory(objId id);
		virtual ~Inventory() { }

		Inventory* setMaxSpace(int maxSpace);
		int getMaxSpace() const;
		int getFreeSpace() const;

		Inventory* setMaxWeight(int maxWeight);
		int getMaxWeight() const;
		int getFreeWeight() const;

		virtual int getSize() const;
		virtual Inventory* setBaseSize(int size);
		virtual int getBaseSize() const;

		virtual int getWeight() const;
		virtual Inventory* setBaseWeight(int weight);
		virtual int getBaseWeight() const;

		void addItem(ObjectPointer itemPtr);
		bool canAdd(ObjectPointer itemPtr);
		void removeItem(ObjectPointer itemPtr);
		bool contains(ObjectPointer itemPtr);

		/**
		 * Makes a string describing what is inside
		 * 
		 * @return a string for AD
		 */
		virtual string getContainingSentence();

		/**
		 * Adds information about size and weight
		 */
		virtual string getDescription() const;

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
		DropAction(string type = "inventory-drop") : MultiTargetAction(type) { }

		virtual void explain(ActionDescriptor* ad);
		virtual bool match(string command, ActionDescriptor* ad);
		virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);
	};
}

#endif	/* INVENTORY_HPP */

