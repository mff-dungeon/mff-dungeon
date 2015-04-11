#ifndef ITEM_HPP
#define	ITEM_HPP

#include "../common.hpp"
#include "Virtual/IDescriptable.hpp"

namespace Dungeon {

	/**
	 * An item can be taken and placed into inventory.
	 */
	class Item : public IDescriptable {
	public:
		Item();
		Item(const objId& id);
		virtual ~Item() { }

		/**
		 * Approximate size in litres/1000 
		 */
		virtual Item* setSize(int size);
		virtual int getSize() const;

		/**
		 * Approximate weight in kg/1000
		 */
		virtual Item* setWeight(int weight);
		virtual int getWeight() const;
	
		bool isPickable() const;
		Item* setPickable(bool pickable);
		bool isDropable() const;
		Item* setDropable(bool dropable);

		virtual void getActions(ActionDescriptor* ad) {}

		/**
		 * Must be inserted to world and be "inside" something to work.
		 */
		virtual Item* respawnEvery(int seconds);

		/**
		 * Allows to add a stat requirement to the item. Doesn't take care of checking,
		 * checking should be done by each item (SpellScroll, Wearable, ...)
		 */
		Item* addStatReq(ObjectPointer reqPtr);
		bool checkStatReqs(ObjectPointer userPtr, ActionDescriptor* ad = nullptr);

		virtual string getDescription() const;
		virtual string getDescriptionSentence();
		virtual string getGroupDescriptionSentence(vector<ObjectPointer> others);

		virtual void registerProperties(IPropertyStorage& storage);
		virtual ObjectPointer onPick(ActionDescriptor* ad) {
			triggerTraps("picked", ad);
			return this;
		}
		virtual ObjectPointer onDrop(ActionDescriptor* ad) {
			triggerTraps("dropped", ad);
			return this;
		}

	private:
		int size;
		int weight;

		bool pickable;
		bool dropable;

		PERSISTENT_DECLARATION(Item, IDescriptable)
	};
};

#endif	/* ITEM_HPP */

