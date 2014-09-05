

#ifndef THORSHAMMER_HPP
#define	THORSHAMMER_HPP

#include <queue>
#include "../common.hpp"
#include "../Action.hpp"
#include "../ActionDescriptor.hpp"
#include "Wearable.hpp"

namespace Dungeon {
    
    /** 
     * Allows it's bearer perform administrative tasks. Beware!
     */
    class ThorsHammer : public Item {
    public:
        ThorsHammer();
        virtual ~ThorsHammer();

        virtual void getActions(ActionList* list, ObjectPointer calee);
		
    private:
        /**
         * TODO his really needs to prevent target from unloading in Splay tree,
         * otherwise it could do terrible things!
         */
        class PropertyEditor : public Action, public IPropertyStorage {
        public:
            PropertyEditor() : Action("property-editor", true) {}
            
            // Action side
            virtual void commit(ActionDescriptor* ad);
            virtual void explain(ActionDescriptor* ad);
            virtual bool matchCommand(string command);
            virtual bool handleException(GameException& exception, ActionDescriptor* ad);


            
            // Property Storage
            virtual IPropertyStorage& have(string& prop, string id, string desc, bool editable);
            virtual IPropertyStorage& have(int& prop, string id, string desc, bool editable);
            virtual IPropertyStorage& have(bool& prop, string id, string desc, bool editable);
            
            void askForNextOne(ActionDescriptor* ad);

        private:
            TextActionDescriptor* ad;
            ObjectPointer target;
            std::queue<string> descriptions;
        };
        
    PERSISTENT_DECLARATION(ThorsHammer, IObject)
    };
}
#endif	/* THORSHAMMER_HPP */

