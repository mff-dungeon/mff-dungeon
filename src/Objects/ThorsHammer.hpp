/* 
 * Allows it's bearer perform administrative tasks. Beware!
 */

#ifndef THORSHAMMER_HPP
#define	THORSHAMMER_HPP

#include "../common.hpp"

namespace Dungeon {
    class ThorsHammer : public IObject {
    public:
        ThorsHammer();
        ThorsHammer(const ThorsHammer& orig);
        virtual ~ThorsHammer();

        virtual void getActions(ActionList* list, IObject* calee);
        virtual void serialize(Archiver &stream);
		
    private:
        
    PERSISTENT_DECLARATION(ThorsHammer, "ThorsHammer")

    };
}
#endif	/* THORSHAMMER_HPP */

