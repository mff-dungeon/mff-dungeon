#ifndef OBJECTGROUP_HPP
#define	OBJECTGROUP_HPP

#include "common.hpp"
#include "GameManager.hpp"
#include "FuzzyStringMatcher.hpp"
#include "Objects/IDescriptable.hpp"

namespace Dungeon {
    
    struct objIdTypeComparator {
        bool operator() (const objId& lhs, const objId& rhs) const {
            // ignore stuff after the last slash char
            string ltype = objId_getType(lhs);
            string rtype = objId_getType(rhs);
            
            return ltype < rtype;
        }
    };
    
               
    typedef multimap<objId, ObjectPointer, objIdTypeComparator> ObjectGroupMap;
        
    /**
     * Helper class for grouping objects of the same type.
     * 
     * Implements exploring, and will implement searching for object
     * best corresponding to given string.
     */
    class ObjectGroup : public ObjectGroupMap {
    public:
        ObjectGroup(ObjectPointer ptr);
        ObjectGroup(const vector<ObjectPointer>& pointers);
        ObjectGroup(ObjectMap map);
        
        /** @deprecated */
        ObjectGroup(IObject *obj);
        
        /** @deprecated */
        ObjectGroup(const vector<IObject *>& objects);
        
        string explore();
        
        ObjectPointer match(string name) {
            FuzzyStringMatcher<ObjectPointer> matcher;
            ObjectGroup::iterator it;
            for (it = this->begin(); it != this->end(); it++) {
                IDescriptable* obj = it->second.safeCast<IDescriptable>();
                if (obj) {
                    matcher.add(obj->getLongName(), it->second);
                    matcher.add(obj->getName(), it->second);
                }
            }
            
            return matcher.find(name);
        }
        
    private:
        ObjectGroupMap::value_type getPair(IObject *obj);
        ObjectGroupMap::value_type getPair(ObjectPointer ptr);

    };
}

#endif /* OBJECTGROUP_HPP */
