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
		Potion *pot;
		
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
		//asaru = (Human*) gm->addNewFigure(new Human("human/asaru@jabbim.cz", "Asaru", "asaru@jabbim.cz"));
		asaru = (Human*) gm->addNewFigure(new Human("human/salmelu@salmelu-i5521", "Asaru", "salmelu@salmelu-i5521"));
		petr = (Human*) gm->addNewFigure(new Human("human/petr.manek@jabbim.com", "CiTrus", "petr.manek@jabbim.com"));
		
		pot = new Potion("item/potion/health1");
		pot->setName("Healing potion")->setLongName("A red potion in glass vial.");
		pot->setPotionType(Potion::PotionType::Healing)->setStrength(200);
		gm->insertObject(pot);
		pot->save(gm);
		
		/*
		 * Init relations
		 */
		gm->createRelation(aearsis, th, R_INVENTORY);
		gm->createRelation(asaru, th, R_INVENTORY);
		gm->createRelation(petr, th, R_INVENTORY);
		
		gm->createRelation(trapRoom, pot, R_INSIDE);
		
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

