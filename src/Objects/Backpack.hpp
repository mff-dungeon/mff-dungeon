#ifndef BACKPACK_HPP
#define	BACKPACK_HPP

#include "../common.hpp"
#include "../ActionDescriptor.hpp"
#include "../Actions/MultiTargetAction.hpp"
#include "Item.hpp"

#define DEFAULT_SPACE 500
#define DEFAULT_WEIGHT 5000

namespace Dungeon {
	
	class Backpack : public Item {
	public:
		Backpack();
		Backpack(objId id);
		virtual ~Backpack();
		
		void setMaxSpace(int maxSpace);
		int getMaxSpace();
		int getFreeSpace();
		
		void setMaxWeight(int maxWeight);
		int getMaxWeight();
		int getFreeWeight();
		
		void addItem(Item* item);
		void removeItem(Item* item);
		
		virtual string getDescriptionSentence();
		virtual string getGroupDescriptionSentence(vector<IDescriptable*> others);
		
		virtual void getActions(ActionList* list, IObject* callee);
		
		virtual void registerProperties(IPropertyStorage& storage);
	private:
		int maxSpace;
		int maxWeight;
		int usedSpace;
		int usedWeight;
		
	PERSISTENT_DECLARATION(Backpack)
	};
	
	class DropAction : public MultiTargetAction {
	public:
		DropAction(string type = "backpack-drop") : MultiTargetAction(type) {}
        
        virtual void explain(ActionDescriptor* ad);
        virtual bool matchCommand(string command);
        virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);

        virtual void commit(ActionDescriptor* ad);
	};
}

#endif	/* BACKPACK_HPP */

