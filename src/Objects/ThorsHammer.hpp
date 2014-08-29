

#ifndef THORSHAMMER_HPP
#define	THORSHAMMER_HPP

#include "../common.hpp"

namespace Dungeon {
    
    /** 
     * Allows it's bearer perform administrative tasks. Beware!
     */
    class ThorsHammer : public IObject {
    public:
        ThorsHammer();
        ThorsHammer(const ThorsHammer& orig);
        virtual ~ThorsHammer();

        virtual void getActions(ActionList* list, IObject* calee);
		
    private:
        
    PERSISTENT_DECLARATION(ThorsHammer)

    };
}
#endif	/* THORSHAMMER_HPP */

