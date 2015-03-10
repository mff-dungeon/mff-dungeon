#ifndef POTION_HPP
#define	POTION_HPP

#include "../common.hpp"
#include "Item.hpp"

namespace Dungeon {

	class Potion : public Item {
	public:

		enum PotionType {
			NoEffect = 0,
			Healing = 1,
			Poison = 2
		};

		Potion() {}
		Potion(const objId& id) : Item(id) { }
		virtual ~Potion() {}

		virtual void getActions(ActionList* list, ObjectPointer callee);

		Potion* setType(PotionType type);
		Potion* setStrength(int strength);
		int getStrength() const {
			return strength;
		}
		PotionType getType() const {
			return type;
		}
		virtual int getSize() const {
			int s = Item::getSize();
			if (s != 0) return s;
			return 100 + getStrength(); // 100 for vial :)
		}
		virtual int getWeight() const {
			int s = Item::getSize();
			if (s != 0) return s;
			return 100 + getStrength() * 2; // Potions are heavier than water
		}

		virtual string getDescription() const;

		virtual void registerProperties(IPropertyStorage& storage);

	private:
		PotionType type = NoEffect;
		int strength = 0;

		PERSISTENT_DECLARATION(Potion, Item)
	};

	class DrinkPotionAction : public MultiTargetAction {
	public:
		DrinkPotionAction(const string& type = "potion-drink") : MultiTargetAction(type) { }

		virtual void explain(ActionDescriptor* ad);
		virtual bool match(const string& command, ActionDescriptor* ad);
		virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);

	};

};

#endif	/* POTION_HPP */

