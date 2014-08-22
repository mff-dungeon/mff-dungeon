#ifndef ITEM_HPP
#define	ITEM_HPP

#include "../common.hpp"
#include "IDescriptable.hpp"
#include "../ActionList.hpp"
#include "../Actions/CallbackAction.hpp"
#include "../ActionDescriptor.hpp"

namespace Dungeon {
	class Item : public IDescriptable{
	public:
		Item();
		Item(objId id);
		virtual ~Item();

        virtual void getActions(ActionList* list, IObject* callee);
		
        virtual string getDescriptionSentence();
        virtual string getGroupDescriptionSentence(vector<IDescriptable *> others);
		
		virtual void serialize(Archiver& stream);
		
	PERSISTENT_DECLARATION(Item)
	};	
	
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

#endif	/* ITEM_HPP */

