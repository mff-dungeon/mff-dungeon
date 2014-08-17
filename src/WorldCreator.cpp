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
		Human *aearsis, *asaru, *petr;
		
		baseRoom = new Room("room/baseRoom");
		baseRoom->setName("Base Camp")
				->setDescription("Nothing much to be found here, but get familiar with basic commands - try to type 'help'.");
		gm->insertObject(baseRoom);
		
		trapRoom = new Room("room/trapRoom");
		trapRoom->setName("Equip room")
				->setDescription("Looks like noone has been there for a long time.");
		gm->insertObject(trapRoom);
		
		gm->insertObject(th = new ThorsHammer());
		aearsis = (Human*) gm->addNewFigure(new Human("human/aearsis@eideo.cz", "Aearsis", "aearsis@eideo.cz"));
		asaru = (Human*) gm->addNewFigure(new Human("human/asaru@jabbim.cz", "Asaru", "asaru@jabbim.cz"));
		petr = (Human*) gm->addNewFigure(new Human("human/petr.manek@jabbim.com", "CiTrus", "petr.manek@jabbim.com"));
		
		/*
		 * Init relations
		 */
		gm->createRelation(aearsis, th, R_INVENTORY);
		gm->createRelation(asaru, th, R_INVENTORY);
		gm->createRelation(petr, th, R_INVENTORY);
		
		this->createDoor("trap-base", baseRoom, trapRoom)
				->setName("wooden door")
				->setLongName("wooden door with simple metal handle")
				->save(gm);
	}
	
	Door* WorldCreator::createDoor(string name, Room* a, Room* b) {
		Door* d = new Door("door/" + name);
		gm->insertObject(d);
		gm->createRelation(a, d, R_INSIDE);
		gm->createRelation(b, d, R_INSIDE);
		return d;
	}

	
	
		
}

