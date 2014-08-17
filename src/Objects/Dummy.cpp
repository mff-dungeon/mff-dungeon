#include "Dummy.hpp"

namespace Dungeon
{

    DummyObject::DummyObject()
    {
        this->setId("dummy");
    }

    void DummyObject::getActions(ActionList* list, IObject *callee)
    {
    }

}

