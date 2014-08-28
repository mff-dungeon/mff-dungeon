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

        virtual void getActions(ActionList* list, IObject* callee);

        virtual string getDescriptionSentence();
        virtual string getGroupDescriptionSentence(vector<IDescriptable *> others);

        virtual void serialize(Archiver& stream);

    PERSISTENT_DECLARATION(Item)
    };
};

#endif	/* ITEM_HPP */

