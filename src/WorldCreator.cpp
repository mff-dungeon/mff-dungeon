#include "WorldCreator.hpp"
#include "GameManager.hpp"

namespace Dungeon {
	
	void WorldCreator::bigBang(GameManager* gm) {
		/**
		 * Init Objects
		 */
		Room* baseRoom;
		ThorsHammer* th;
		Alive *aearsis, *asaru, *petr;
		gm->insertObject(baseRoom = new Room("room/baseRoom"));
		gm->insertObject(th = new ThorsHammer());
		gm->insertObject(aearsis = new Alive("human/aearsis@eideo.cz"));
		gm->insertObject(asaru = new Alive("human/asaru@jabbim.cz"));
		gm->insertObject(petr = new Alive("human/petr.manek@jabbim.com"));
		
		/*
		 * Init relations
		 */
		gm->createRelation(baseRoom, aearsis, R_INSIDE);
		gm->createRelation(baseRoom, asaru, R_INSIDE);
		gm->createRelation(baseRoom, petr, R_INSIDE);
		gm->createRelation(aearsis, th, R_INVENTORY);
		gm->createRelation(asaru, th, R_INVENTORY);
		gm->createRelation(petr, th, R_INVENTORY);
	}
		
}

