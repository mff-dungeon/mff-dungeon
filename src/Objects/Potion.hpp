#ifndef POTION_HPP
#define	POTION_HPP

#include "../common.hpp"
#include "Item.hpp"

namespace Dungeon {
	
    /**
     * IDrinkable ;)
     */
	
    class Potion : public Item {
    public:
        enum PotionType {
            NoEffect = 0,
            Healing = 1
        };

        Potion();
        Potion(objId id);
        virtual ~Potion();

        virtual void getActions(ActionList* list, ObjectPointer callee);

        Potion* setType(PotionType type);
        Potion* setStrength(int strength);

        int getStrength() const {
            return strength;
        }

        PotionType getType() const {
            return type;
        }

        virtual int getSize() {
            int s = Item::getSize();
            if (s != 0) return s;
            return 100 + getStrength(); // 100 for vial :)
        }
        
        virtual int getWeight() {
            int s = Item::getSize();
            if (s != 0) return s;
            return 100 + getStrength() * 2; // Potions are heavier than water
        }


        virtual void registerProperties(IPropertyStorage& storage);

    private:
            PotionType type = NoEffect;
            int strength = 0;

    PERSISTENT_DECLARATION(Potion, Item)
    };	
    
    class DrinkPotionAction : public MultiTargetAction {
    public:
        DrinkPotionAction(string type = "potion-drink") : MultiTargetAction(type) {}
        
        virtual void explain(ActionDescriptor* ad);
        virtual bool matchCommand(string command);
        virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);

        virtual void commit(ActionDescriptor* ad);

    };
    
};

#endif	/* POTION_HPP */

