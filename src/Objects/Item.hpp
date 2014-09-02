#ifndef ITEM_HPP
#define	ITEM_HPP

#include "../common.hpp"
#include "IDescriptable.hpp"
#include "../ActionList.hpp"

namespace Dungeon {
    
    /**
     * An item can be taken and placed into inventory.
     */
    class Item : public IDescriptable{
    public:
        Item();
        Item(objId id);
        virtual ~Item();
		
        /**
         * Approximate size in litres/1000 
         */
        virtual Item* setSize(int size);
        virtual int getSize();
        
        /**
         * Approximate weight in kg/1000
         */
        virtual Item* setWeight(int weight);
        virtual int getWeight();

        virtual bool isPickable();
        virtual Item* setPickable(bool pickable);
        virtual bool isDropable();
        virtual Item* setDropable(bool dropable);

        virtual void getActions(ActionList* list, IObject* callee);

        virtual string getDescriptionSentence();
        virtual string getGroupDescriptionSentence(vector<IDescriptable *> others);
		
		virtual void registerProperties(IPropertyStorage& storage);
		
	private:
		int size;
		int weight;
		
		bool pickable;
		bool dropable;

    PERSISTENT_DECLARATION(Item)
    };
};

#endif	/* ITEM_HPP */

