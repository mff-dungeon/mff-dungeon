#include "ObjectGroup.hpp"
#include "Objects/IDescriptable.hpp"

namespace Dungeon
{
    ObjectGroup::ObjectGroup(IObject *obj) : ObjectGroupMap() {
        this->insert(getPair(obj));
    }
    
    ObjectGroup::ObjectGroup(ObjectPointer ptr) : ObjectGroupMap() {
        this->insert(getPair(ptr));
    }
    
    ObjectGroup::ObjectGroup(const vector<IObject *>& objects) : ObjectGroupMap() {
        for (IObject *obj : objects) {
            this->insert(getPair(obj));
        }
    }
    
    ObjectGroup::ObjectGroup(const vector<ObjectPointer>& pointers) : ObjectGroupMap() {
        for (ObjectPointer ptr : pointers) {
            this->insert(getPair(ptr));
        }
    }
    
    ObjectGroup::ObjectGroup(ObjectMap map) : ObjectGroupMap(map.begin(), map.end()) {
        
    }
    
    ObjectGroupMap::value_type ObjectGroup::getPair(IObject *obj) {
        return ObjectGroupMap::value_type(obj->getId(), obj->getObjectPointer());
    }
    
    ObjectGroupMap::value_type ObjectGroup::getPair(ObjectPointer ptr) {
        return ObjectGroupMap::value_type(ptr.getId(), ptr);
    }
    
    string ObjectGroup::explore() {
        string sentence = "";
        ObjectGroup::iterator m_it, s_it;
        
        for (m_it = this->begin(); m_it != this->end(); m_it = s_it) {
            objId firstIdentifier = m_it->first;
            string objectType = objId_getType(firstIdentifier);
            
            if (m_it->second->instanceOf(IDescriptable)) {
                pair<ObjectGroup::iterator, ObjectGroup::iterator> keyRange = this->equal_range(firstIdentifier);
                vector<ObjectPointer> sameTypeObjects;
                
                for (s_it = keyRange.first; s_it != keyRange.second; s_it++) {
                    sameTypeObjects.push_back(s_it->second);
                }
                
                if (sameTypeObjects.empty()) {
                    continue;
                } else if (sameTypeObjects.size() == 1) {
                    sentence += sameTypeObjects.front().unsafeCast<IDescriptable>()->getDescriptionSentence() + " ";
                } else {
                    sentence += sameTypeObjects.front().unsafeCast<IDescriptable>()->getGroupDescriptionSentence(sameTypeObjects) + " ";
                }
            } else {
                sentence += "There is an object (" + m_it->second.getId() + "). ";
            }
        }
        
        return sentence;
    }
    
    
}
