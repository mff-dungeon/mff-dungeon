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
        ObjectGroup(IObject *obj);
        ObjectGroup(ObjectPointer ptr);
        ObjectGroup(const vector<IObject *>& objects);
        ObjectGroup(const vector<ObjectPointer>& pointers);
        ObjectGroup(ObjectMap map);
        
        string explore();
        
        ObjectPointer match(string name) {
            FuzzyStringMatcher<ObjectPointer> matcher;
            ObjectGroup::iterator it;
            for (it = this->begin(); it != this->end(); it++) {
                IObject* obj = it->second.get();
                if (obj->instanceOf("IDescriptable")) {
                    IDescriptable* dobj = (IDescriptable*) obj;
                     matcher.add(dobj->getLongName(), it->second);
                     matcher.add(dobj->getName(), it->second);
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
