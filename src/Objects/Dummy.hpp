#ifndef DUMMY_HPP
#define	DUMMY_HPP

#include "../common.hpp"

namespace Dungeon {

    class DummyObject : public IObject {
    public:
        DummyObject();
        virtual void getActions(ActionList* list, IObject *callee);
    };
	
}

#endif	/* DUMMY_HPP */

