#include "ObjectGroup.hpp"
#include <sstream>

namespace Dungeon
{
    ObjectGroup::ObjectGroup(GameManager *gm, IObject *obj) : multimap<objId, ObjectPointer*, objIdTypeComparator>() {
        this->insert(getPair(gm, obj));
    }
    
    ObjectGroup::ObjectGroup(ObjectPointer *ptr) : multimap<objId, ObjectPointer*, objIdTypeComparator>() {
        this->insert(getPair(ptr));
    }
    
    ObjectGroup::ObjectGroup(GameManager *gm, const vector<IObject *>& objects) : multimap<objId, ObjectPointer*, objIdTypeComparator>() {
        for (IObject *obj : objects) {
            this->insert(getPair(gm, obj));
        }
    }
    
    ObjectGroup::ObjectGroup(const vector<ObjectPointer *>& pointers) : multimap<objId, ObjectPointer*, objIdTypeComparator>() {
        for (ObjectPointer *ptr : pointers) {
            this->insert(getPair(ptr));
        }
    }
    
    ObjectGroup::ObjectGroup(ObjectMap map) : multimap<objId, ObjectPointer*, objIdTypeComparator>(map.begin(), map.end()) {
        
    }
    
    pair<objId, ObjectPointer*> ObjectGroup::getPair(GameManager *gm, IObject *obj) {
        return pair<objId, ObjectPointer*>(obj->getId(), new ObjectPointer(gm, obj->getId()));
    }
    
    pair<objId, ObjectPointer*> ObjectGroup::getPair(ObjectPointer *ptr) {
        return pair<objId, ObjectPointer*>(ptr->getId(), ptr);
    }
    
    string ObjectGroup::explore() {
        string sentence = "";
        ObjectGroup::iterator m_it, s_it;
        
        for (m_it = this->begin(); m_it != this->end(); m_it = s_it) {
            objId firstIdentifier = (*m_it).first;
            string objectType = objId_getType(firstIdentifier);
            
            if ((*m_it).second->get()->isDescriptable()) {
                pair<ObjectGroup::iterator, ObjectGroup::iterator> keyRange = this->equal_range(firstIdentifier);
                vector<IDescriptable *> sameTypeObjects;
                
                for (s_it = keyRange.first; s_it != keyRange.second; s_it++) {
                    sameTypeObjects.push_back((IDescriptable *)(*s_it).second->get());
                }
                
                if (sameTypeObjects.empty()) {
                    continue;
                } else if (sameTypeObjects.size() == 1) {
                    sentence += sameTypeObjects.front()->getDescriptionSentence() + " ";
                } else {
                    sentence += sameTypeObjects.front()->getGroupDescriptionSentence(sameTypeObjects) + " ";
                }
            } else {
                sentence += "There is an object (" + (*m_it).second->getId() + "). ";
            }
        }
        
        return sentence;
    }
}
