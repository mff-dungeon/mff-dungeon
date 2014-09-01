#include "WorldCreator.hpp"
#include "GameManager.hpp"

namespace Dungeon {
	WorldCreator::WorldCreator(GameManager* gm) {
		this->gm = gm;
	}
	
	void WorldCreator::initAdmins() {
		Human *aearsis, *asaru, *petr;
		ThorsHammer* th;
		gm->insertObject(th = new ThorsHammer());
		aearsis = (Human*) gm->addNewFigure(new Human("human/aearsis@eideo.cz", "Aearsis", "aearsis@eideo.cz"));
		gm->createRelation(aearsis, th, R_INVENTORY);
		asaru = (Human*) gm->addNewFigure(new Human("human/asaru@jabbim.cz", "Salmelu", "asaru@jabbim.cz"));
		gm->createRelation(asaru, th, R_INVENTORY);
		petr = (Human*) gm->addNewFigure(new Human("human/petr.manek@jabbim.com", "CiTrus", "petr.manek@jabbim.com"));
		gm->createRelation(petr, th, R_INVENTORY);
		
		Backpack* aearsisPack = new Backpack("item/backpack/" + RANDID);
		aearsisPack->setDropable(false)
			->setPickable(false)
			->setName("Leather backpack")
			->setLongName("A nice and new leather backpack")
			->save();
		Backpack* asaruPack = new Backpack("item/backpack/" + RANDID);
		asaruPack->setDropable(false)
			->setPickable(false)
			->setName("Leather backpack")
			->setLongName("A nice and new leather backpack")
			->save();
		Backpack* petrPack = new Backpack("item/backpack/" + RANDID);
		petrPack->setDropable(false)
			->setPickable(false)
			->setName("Leather backpack")
			->setLongName("A nice and new leather backpack")
			->save();
		gm->createRelation(aearsis, aearsisPack, R_INVENTORY);
		gm->createRelation(asaru, asaruPack, R_INVENTORY);
		gm->createRelation(petr, petrPack, R_INVENTORY);
	}

	void WorldCreator::bigBang() {
		/**
		 * Init Objects
		 */
		Room *baseRoom, *trapRoom;
		
		baseRoom = new Room("room/baseRoom");
		baseRoom->setName("Base Camp")
				->setDescription("Nothing much to be found here, but get familiar with basic commands - try to type 'help'.");
		gm->insertObject(baseRoom);
		
		initAdmins();
		
		trapRoom = new Room("room/trapRoom");
		trapRoom->setName("Equip room")
				->setDescription("Looks like noone has been there for a long time.");
		gm->insertObject(trapRoom);
		
		createObject<Potion>("item/potion/" + RANDID, trapRoom)
			->setType(Potion::PotionType::Healing)
			->setStrength(100)
			->setName("Blue potion")
			->setLongName("A blue potion in glass vial.")
			->save();

		createObject<Potion>("item/potion/" + RANDID, trapRoom)
			->setType(Potion::PotionType::Healing)
			->setStrength(100)
			->setName("Green potion")
			->setLongName("A green potion in gold vial.")
			->save();

		createObject<Potion>("item/potion/" + RANDID, trapRoom)
			->setType(Potion::PotionType::Healing)
			->setStrength(200)
			->setName("Red potion")
			->setLongName("A red potion in silver vial.")
			->save();
		/*
		 * Init relations
		 */
		
		this->createDoor("trap-base", baseRoom, trapRoom)
				->setName("wooden door")
				->setLongName("wooden door with simple metal handle")
				->save();
	}
	
	Door* WorldCreator::createDoor(string name, Room* a, Room* b, bool two_way) {
		Door* d = new Door("door/" + name);
		gm->insertObject(d);
		gm->createRelation(a, d, R_INSIDE);
		gm->createRelation(d, b, R_TARGET);
		if (two_way) {
			gm->createRelation(b, d, R_INSIDE);
			gm->createRelation(d, a, R_TARGET);
		}
		return d;
	}

	
	
		
}

