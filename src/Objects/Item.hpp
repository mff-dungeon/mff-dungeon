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
        virtual int getSize() const;
        
        /**
         * Approximate weight in kg/1000
         */
        virtual Item* setWeight(int weight);
        virtual int getWeight() const;

        virtual bool isPickable() const;
        virtual Item* setPickable(bool pickable);
        virtual bool isDropable() const;
        virtual Item* setDropable(bool dropable);

        virtual void getActions(ActionList* list, ObjectPointer callee);
        
        virtual void examine(ActionDescriptor* ad);

        /**
         * Must be inserted to world and be "inside" something to work.
         */
        virtual Item* respawnEvery(int seconds);

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
    
    class ExamineEction : public MultiTargetAction {
    public:
        ExamineEction() : MultiTargetAction("examine") { }

        virtual bool matchCommand(string command);
        virtual void commit(ActionDescriptor* ad);
        virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);
    };
};

#endif	/* ITEM_HPP */

