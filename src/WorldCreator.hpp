#ifndef WORLDCREATOR_HPP
#define	WORLDCREATOR_HPP

#include "common.hpp"
#include "dynamic.hpp"
#include "Cloner.hpp"
#include "Dropper.hpp"

namespace Dungeon {
    /**
     * Dedicated static class to init the world
     */
    class WorldCreator {
    public:
        WorldCreator(GameManager* gm);
        void initAdmins();
		void initTemplates();
        virtual void bigBang();
        
    protected:
        GameManager* gm;
        Door* createDoor(string name, Location* a, Location* b, bool two_way = true);
        
        template<typename T>
        T* createObject(string id, ObjectPointer location = nullptr) {
            T* ptr = new T(id);
            gm->insertObject(ptr);
            if (!!location) gm->createRelation(location, ptr, R_INSIDE);
            return ptr;
        }
		
		ObjectPointer deepCloneTemplate(ObjectPointer tmp, IObject* location = NULL) {
			ObjectPointer clone = Cloner::deepClone(tmp);
			if(location) clone->setSingleRelation(R_INSIDE, location, Relation::Slave);
			return clone;
		}
		
		ObjectPointer shallowCloneTemplate(ObjectPointer tmp, IObject* location = NULL) {
			ObjectPointer clone = Cloner::shallowClone(tmp);
			if(location) clone->setSingleRelation(R_INSIDE, location, Relation::Slave);
			return clone;
		}
		
	private:
		map<objId, ObjectPointer> templates;
    };

}

#endif	/* WORLDCREATOR_HPP */

