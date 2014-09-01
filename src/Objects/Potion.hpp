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

        virtual void getActions(ActionList* list, IObject* callee);

        Potion* setType(PotionType type);
        Potion* setStrength(int strength);

        int getStrength() const {
            return strength;
        }

        PotionType getType() const {
            return type;
        }

        virtual void registerProperties(IPropertyStorage& storage);



    private:
            PotionType type;
            int strength;

    PERSISTENT_DECLARATION(Potion)
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

