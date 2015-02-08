#ifndef BOARD_HPP
#define	BOARD_HPP

#include "../../common.hpp"
#include "../Interactive.hpp"

namespace Dungeon {

    class Board : public Interactive {
    public:

        Board() {}
        Board(objId id) : Interactive(id) {}
        virtual ~Board() { }
        
        virtual void getActions(ActionList* list, ObjectPointer callee);

        PERSISTENT_DECLARATION(Board, Interactive)

    };
}

#endif

