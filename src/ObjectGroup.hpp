#ifndef OBJECTGROUP_HPP
#define	OBJECTGROUP_HPP

#include "common.hpp"
#include "GameManager.hpp"

namespace Dungeon {
    
    struct objIdTypeComparator {
        bool operator() (const objId& lhs, const objId& rhs) const {
            // ignore stuff after the slash char
            string ltype = objId_getType(lhs);
            string rtype = objId_getType(rhs);
            
            return ltype < rtype;
        }
    };
    
    class ObjectGroup : public multimap<objId, ObjectPointer*, objIdTypeComparator> {
    public:
        ObjectGroup(GameManager *gm, IObject *obj);
        ObjectGroup(ObjectPointer *ptr);
        ObjectGroup(GameManager *gm, const vector<IObject *>& objects);
        ObjectGroup(const vector<ObjectPointer *>& pointers);
        ObjectGroup(ObjectMap map);
        
        string explore();
        
    private:
        pair<objId, ObjectPointer*> getPair(GameManager *gm, IObject *obj);
        pair<objId, ObjectPointer*> getPair(ObjectPointer *ptr);

    };
}

#endif /* OBJECTGROUP_HPP */
