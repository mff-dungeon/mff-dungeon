#include "WorldCreator.hpp"
#include "GameManager.hpp"

namespace Dungeon {
	WorldCreator::WorldCreator(GameManager* gm) {
		this->gm = gm;
	}
	
	void WorldCreator::initAdmins() {
		// Thor's hammer should have special relation, as it is not wearable. It would ruin the world.
		// Also, the admin won't be able to test the game then
		Human *aearsis, *asaru, *petr;
		ThorsHammer* th;
		gm->insertObject(th = new ThorsHammer());
		aearsis = (Human*) gm->addNewFigure(new Human("human/aearsis@eideo.cz", "Aearsis", "aearsis@eideo.cz"));
		gm->createRelation(aearsis, th, "special-th");
		asaru = (Human*) gm->addNewFigure(new Human("human/asaru@jabbim.cz", "Salmelu", "asaru@jabbim.cz"));
		gm->createRelation(asaru, th, "special-th");
		petr = (Human*) gm->addNewFigure(new Human("human/petr.manek@jabbim.com", "CiTrus", "petr.manek@jabbim.com"));
		gm->createRelation(petr, th, "special-th");
		
		// TODO - zvetsit batohy
	}

	void WorldCreator::bigBang() {
		/**
		 * Init Objects
		 */
		Room *baseRoom, *trapRoom, *darkRoom;
		
		baseRoom = new Room("room/baseRoom");
		baseRoom->setName("Base Camp")
				->setDescription("Nothing much to be found here, but get familiar with basic commands - try to type 'help'.");
		gm->insertObject(baseRoom);
		
		initAdmins();
		
		trapRoom = new Room("room/trapRoom");
		trapRoom->setName("Equip room")
				->setDescription("Looks like noone has been there for a long time.");
		gm->insertObject(trapRoom);
		
		darkRoom = new Room("room/darkRoom");
		darkRoom->setName("Dark room")
				->setDescription("A scary room with a lack of light.");
		gm->insertObject(darkRoom);
		
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
		
		createObject<Inventory>("item/backpack/" + RANDID, trapRoom)
			->setMaxSpace(30000)
			->setMaxWeight(50000)
			->setSlot(Wearable::Backpack)
			->setDefenseBonus(2)
			->setSize(5000)
			->setWeight(2500)
			->setName("Hard leather backpack")
			->setLongName("A more solid leather backpack.")
			->save();
		
		createObject<Wearable>("item/weapon/" + RANDID, trapRoom)
			->setAttackBonus(6)
			->setSlot(Wearable::Weapon)
			->setSize(3000)
			->setWeight(5000)
			->setName("Wooden club")
			->setLongName("A weak wooden club.")
			->save();
		
		createObject<Creature>("creature/spider/" + RANDID, darkRoom)
			->setAttack(3)
			->setDefense(2)
			->setMaxHp(100)
			->setCurrentHp(100)
			->setRespawnInterval(20)
			->setName("Small spider")
			->setLongName("A small brown spider")
			->save();
		/*
		 * Init relations
		 */
		
		IObject* d = this->createDoor("trap-base", baseRoom, trapRoom)
				->setGoThroughMessage("You've gone through that door. It made a funny noise.")
				->setName("wooden door")
				->setLongName("wooden door with simple metal handle")
				->save();
		this->createDoor("dark-base", baseRoom, darkRoom)
				->setName("narrow tunnel")
				->setLongName("a narrow tunnel with no visible ending")
				->save();
		
		Trap* t = createObject<DemoTrap>("trap/demo");
		gm->createRelation(t, d, Trap::getRelation("go-through"));
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

