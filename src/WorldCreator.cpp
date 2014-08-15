#include "WorldCreator.hpp"
#include "GameManager.hpp"

namespace Dungeon {
	WorldCreator::WorldCreator(GameManager* gm) {
		this->gm = gm;
	}

	void WorldCreator::bigBang() {
		/**
		 * Init Objects
		 */
		Room *baseRoom, *trapRoom;
		ThorsHammer* th;
		Alive *aearsis, *asaru, *petr;
		gm->insertObject(baseRoom = new Room("room/baseRoom"));
		gm->insertObject(trapRoom = new Room("room/trapRoom"));
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
		
		this->createDoor("trap-base", baseRoom, trapRoom);
	}
	
	Door* WorldCreator::createDoor(string name, Room* a, Room* b) {
		Door* d = new Door("door/" + name);
		gm->insertObject(d);
		gm->createRelation(a, d, R_INSIDE);
		gm->createRelation(b, d, R_INSIDE);
		return d;
	}

	
	
		
}

