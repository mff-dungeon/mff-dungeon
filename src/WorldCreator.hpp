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
        Door* createDoor(string name, Room* a, Room* b, bool two_way = true);
        
        template<typename T>
        T* createObject(string id, IObject* location = NULL) {
            T* ptr = new T(id);
            gm->insertObject(ptr);
            if (location) gm->createRelation(location, ptr, R_INSIDE);
            return ptr;
        }
		
		// TODO: a nicer way to deal with templateId
		ObjectPointer cloneTemplate(ObjectPointer tmp, IObject* location = NULL) {
			ObjectPointer clone = Cloner::deepClone(tmp);
			if(location) clone->setSingleRelation(R_INSIDE, location, Relation::Slave);
			return clone;
		}
		
	private:
		map<objId, ObjectPointer> templates;
    };

}

#endif	/* WORLDCREATOR_HPP */

