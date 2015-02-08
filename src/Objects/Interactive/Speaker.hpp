#ifndef SPEAKER_HPP
#define	SPEAKER_HPP

#include "../../common.hpp"
#include "../Interactive.hpp"

namespace Dungeon {

    class Speaker : public Interactive {
    public:

        Speaker() {}
        Speaker(objId id) : Interactive(id) {}
        virtual ~Speaker() { }
        
        virtual void getActions(ActionList* list, ObjectPointer callee);

        PERSISTENT_DECLARATION(Speaker, Interactive)

    };
}

#endif

