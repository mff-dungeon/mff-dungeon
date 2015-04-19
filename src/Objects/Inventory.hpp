#ifndef INVENTORY_HPP
#define	INVENTORY_HPP

#include "../common.hpp"
#include "../Game/ActionDescriptor.hpp"
#include "../Actions/MultiTargetAction.hpp"
#include "Wearable.hpp"
#include "Resource.hpp"
#include "Human.hpp"

namespace Dungeon {

	class Inventory : public Wearable {
	public:
		const int DEFAULT_SPACE = 20000; // 20 litres
		const int DEFAULT_WEIGHT = 20000; // 20 kg
		Inventory();
		Inventory(const objId& id);
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

		ObjectPointer addItem(ObjectPointer itemPtr);
		bool canAdd(ObjectPointer itemPtr);
		ObjectPointer removeItem(ObjectPointer itemPtr, int amount = 1);
		bool contains(ObjectPointer itemPtr);

		/*
		 * The following methods return true, if the resource was added,
		 * false if it couldn't be added/removed due to inventory size/weight limits
         */
		int getResourceQuantity(Resource::ResourceType type);
		bool changeResourceQuantity(Resource::ResourceType type, int deltaQuantity);
		bool addResource(Resource::ResourceType type, int added);
		bool removeResource(Resource::ResourceType type, int removed);
		bool setResource(Resource::ResourceType type, int newAmount);

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

		virtual void getActions(ActionDescriptor* ad);
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
		DropAction(const string& type = "inventory-drop") : MultiTargetAction(type) { }

		virtual void explain(ActionDescriptor* ad);
		virtual bool match(const string& command, ActionDescriptor* ad);
		virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);

	private:
		int amount = 0;
		CaptureMatcher captureMatcher;
	};
}

#endif	/* INVENTORY_HPP */

