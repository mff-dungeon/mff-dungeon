#include "Dummy.hpp"

namespace Dungeon
{

    DummyObject::DummyObject()
    {
        this->setId("dummy");
    }

    void DummyObject::getActions(ActionList* list, IObject *callee)
    {
        list->push_back(new Action("dummy"));
    }

}

