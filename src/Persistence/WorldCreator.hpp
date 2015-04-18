#ifndef WORLDCREATOR_HPP
#define	WORLDCREATOR_HPP

#include "../common.hpp"
#include "../dynamic.hpp"
#include "../Objects/Virtual/Dropper.hpp"
#include "../Objects/Virtual/Stats.hpp"
#include "../Game/GameManager.hpp"
#include "Cloner.hpp"

namespace Dungeon {
    /**
     * Dedicated static class to init the world
     */
    class WorldCreator {
    public:
        WorldCreator(GameManager* gm);
        void initAdmins();
		void initTemplates();
		void initRooms();
		void initDoors();
		void initObjects();
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

		ObjectPointer statReq(Human::Stats stat, int value) {
			StatReq* req = new StatReq("statreq/rand/" + RANDID);
			gm->insertObject(req);
			req->setStat(stat)->setValue(value)->save();
			return req;
		}

		ObjectPointer deepCloneTemplate(ObjectPointer tmp, Base* location = NULL) {
			ObjectPointer clone = Cloner::deepClone(tmp);
			if(location) clone->setSingleRelation(R_INSIDE, location, Relation::Slave);
			return clone;
		}

		ObjectPointer shallowCloneTemplate(ObjectPointer tmp, Base* location = NULL) {
			ObjectPointer clone = Cloner::shallowClone(tmp);
			if(location) clone->setSingleRelation(R_INSIDE, location, Relation::Slave);
			return clone;
		}

	private:
		map<string, ObjectPointer> templates;
		map<string, Location*> rooms;
    };

}

#endif	/* WORLDCREATOR_HPP */

