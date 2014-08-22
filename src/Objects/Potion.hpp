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
		
		Potion* setPotionType(PotionType type);
		Potion* setStrength(int strength);
		
		virtual void serialize(Archiver& stream);
        
	private:
		PotionType type;
		int strength;
		
        PERSISTENT_DECLARATION(Potion)
	};	
    
};

#endif	/* POTION_HPP */

