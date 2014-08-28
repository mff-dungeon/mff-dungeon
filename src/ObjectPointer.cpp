#include "ObjectPointer.hpp"

namespace Dungeon
{

    ObjectPointer::ObjectPointer(GameManager *gm, objId id)
    {
        this->gm = gm;
        this->id = id;
    }

    IObject* ObjectPointer::get()
    {
        return gm->getObject(this->id);
    }

    objId ObjectPointer::getId()
    {
        return this->id;
    }
	
	bool ObjectPointer::isLoaded() {
		return gm->hasObjectLoaded(id);
	}


}
