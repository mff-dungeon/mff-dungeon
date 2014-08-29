

#ifndef THORSHAMMER_HPP
#define	THORSHAMMER_HPP

#include <queue>
#include "../common.hpp"
#include "../Action.hpp"
#include "../ActionDescriptor.hpp"

namespace Dungeon {
    
    /** 
     * Allows it's bearer perform administrative tasks. Beware!
     */
    class ThorsHammer : public IObject {
    public:
        ThorsHammer();
        virtual ~ThorsHammer();

        virtual void getActions(ActionList* list, IObject* calee);
		
    private:
        class PropertyEditor : public Action, public IPropertyStorage {
        public:
            PropertyEditor() : Action("property-editor", true) {}
            
            // Action side
            virtual void commit(ActionDescriptor* ad);
            virtual void explain(ActionDescriptor* ad);
            virtual bool matchCommand(string command);
            
            // Property Storage
            virtual IPropertyStorage& have(string& prop, string desc, bool editable);
            virtual IPropertyStorage& have(int& prop, string desc, bool editable);
            virtual IPropertyStorage& have(bool& prop, string desc, bool editable);
            
            void askForNextOne(ActionDescriptor* ad);

        private:
            TextActionDescriptor* ad;
            IObject* target;
            std::queue<string> descriptions;
        };
        
    PERSISTENT_DECLARATION(ThorsHammer)
    };
}
#endif	/* THORSHAMMER_HPP */

