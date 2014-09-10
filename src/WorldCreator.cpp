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
		gm->createRelation(aearsis, th, "special-th");
		asaru = (Human*) gm->addNewFigure(new Human("human/asaru@jabbim.cz", "Salmelu", "asaru@jabbim.cz"));
		gm->createRelation(asaru, th, "special-th");
		petr = (Human*) gm->addNewFigure(new Human("human/petr.manek@jabbim.com", "CiTrus", "petr.manek@jabbim.com"));
		gm->createRelation(petr, th, "special-th");

		// TODO - zvetsit batohy
	}

	void WorldCreator::initTemplates() {
		Room* templateRoom = createObject<Room>("room/templateRoom")
				->setName("Template storage")
				->setDescription("Just a room to keep all the templates at one place.")
				->save().unsafeCast<Room>();
		
		templates["potion/greenhealing"] = createObject<Potion>("template/potion/greenhealing/1", templateRoom)
				->setType(Potion::PotionType::Healing)
				->setStrength(100)
				->setName("Green potion")
				->setLongName("A green potion in gold vial.")
				->setDescription("The vial is untouched, and the color of the liquid is some kind of scary.")
				->save();
		
		templates["creature/smallspider"] = createObject<Creature>("template/creature/smallspider/1", templateRoom)
				->attachDrop(createObject<Dropper>("dropper/smallspider/1/1")
					->setChance(Dropper::Always)
					->setMin(1)
					->setMax(1)
					->setItem(templates["potion/greenhealing"])
					->save())
				->setAttack(3)
				->setDefense(2)
				->setMaxHp(100)
				->setCurrentHp(100)
				->setRespawnInterval(90)
				->setName("Small spider")
				->setLongName("A small brown spider")
				->setDescription("On closer look, it is cute. But I won't touch it!")
				->save();
	}


	void WorldCreator::bigBang() {
		/**
		 * Base
		 */
		Room* baseRoom = createObject<Room>("room/baseRoom")
				->setRespawnable(true)
				->setName("Base Camp")
				->setDescription("Nothing much to be found here, but get familiar with basic commands - try to type 'help'.")
				->save().unsafeCast<Room>();

		initAdmins();
		initTemplates();

		AttackTrap* autoAttack = createObject<AttackTrap>("trap/attack");

		/*
		 ************************     Equip room		************************
		 */

		Room* equipRoom = createObject<Room>("room/equipRoom")
				->setName("Equip room")
				->setDescription("Looks like noone has been there for a long time.")
				->save().unsafeCast<Room>();

		createDoor("equip-base", baseRoom, equipRoom)
				->setGoThroughMessage("You've gone through that door. It made a funny noise. ")
				->setName("wooden door")
				->setLongName("wooden door with simple metal handle")
				->save();

		createObject<Potion>("potion/blue/" + RANDID, equipRoom)
				->setType(Potion::PotionType::Poison)
				->setStrength(100)
				->setName("Blue potion")
				->setLongName("a blue potion in glass vial")
				->setDescription("It may look like water for some people. ")
				->save();

		createObject<Potion>("potion/redhealing/" + RANDID, equipRoom)
				->setType(Potion::PotionType::Healing)
				->setStrength(200)
				->setName("Red potion")
				->setLongName("a red potion in silver vial")
				->setDescription("The vial has some scratches, but the color of the liquid is just beautiful. ")
				->save();

		createObject<Inventory>("inventory/hardleatherbackpack/" + RANDID, equipRoom)
				->setMaxSpace(30000)
				->setMaxWeight(50000)
				->setSlot(Wearable::Backpack)
				->setDefenseBonus(2)
				->setSize(5000)
				->setWeight(2500)
				->setName("Hard leather backpack")
				->setLongName("a more solid leather backpack")
				->save();

		createObject<Wearable>("wearable/woodenclub/" + RANDID, equipRoom)
				->setAttackBonus(6)
				->setSlot(Wearable::Weapon)
				->setSize(3000)
				->setWeight(5000)
				->setName("Wooden club")
				->setLongName("a weak wooden club")
				->setDescription("It looks like a candy next to a sword, but still better than fighting with lollypop. ")
				->save();
		
		createObject<Wearable>("wearable/woodenclub/" + RANDID, equipRoom)
				->setAttackBonus(6)
				->setSlot(Wearable::Weapon)
				->setSize(3000)
				->setWeight(5000)
				->setName("Wooden club")
				->setLongName("a weak wooden club")
				->setDescription("It looks like a candy next to a sword, but still better than fighting with lollypop. ")
				->save();

		/*
		 ************************     Dark room		************************
		 */

		Room* darkRoom = createObject<Room>("room/darkRoom")
				->setName("Dark room")
				->setDescription("a scary room with a lack of light.")
				->attachTrap(autoAttack, "inside")
				->save().unsafeCast<Room>();
		
		cloneTemplate(templates["creature/smallspider"], darkRoom);
		cloneTemplate(templates["creature/smallspider"], darkRoom);

		this->createDoor("dark-base", baseRoom, darkRoom)
				->setGoThroughMessage("You've crawled through that tunnel. It smelled bad. ")
				->setName("narrow tunnel")
				->setLongName("a narrow tunnel with no visible ending")
				->save();

		/*
		 ************************     Boss room		************************
		 */
		Room* bossRoom = createObject<Room>("room/bossRoom")
				->setName("Round room")
				->setDescription("Prepare for the fight!")
				->save().unsafeCast<Room>();

		this->createDoor("dark-boss", bossRoom, darkRoom)
				->setGoThroughMessage("You can't get rid of that strange feeling. ")
				->setName("Crevice")
				->setLongName("hole just big enough to fit in")
				->save();
		
		 createObject<Creature>("creature/megaspider/" + RANDID, bossRoom)
				->attachDrop(createObject<Dropper>("dropper/megaspider/1")
					->setChance(Dropper::Always)
					->setMin(1)
					->setMax(1)
					->setItem(createObject<Wearable>("template/wearable/ironclub/1")
							->setAttackBonus(20)
							->setDefenseBonus(3)
							->setSlot(Wearable::Weapon)
							->setSize(5000)
							->setWeight(10000)
							->setName("Iron club")
							->setLongName("a strong iron club")
							->setDescription("Finally a weapon!")
							->save())
					->save())
				->setAttack(8)
				->setDefense(4)
				->setMaxHp(300)
				->setCurrentHp(300)
				->setRespawnInterval(9000)
				->setName("Big spider")
				->setLongName("A big ugly black spider")
				->setDescription("You would even look closer?!")
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

