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
		aearsis = gm->addNewFigure(new Human("human/aearsis@eideo.cz", "Aearsis", "aearsis@eideo.cz"))
				->setWeaponName("powerful hands of administrator")
				->setSingleRelation("special-th", th)
				->save().unsafeCast<Human>();
		
		asaru = gm->addNewFigure(new Human("human/asaru@jabbim.cz", "Salmelu", "asaru@jabbim.cz"))
				->setWeaponName("powerful hands of administrator")
				->setSingleRelation("special-th", th)
				->save().unsafeCast<Human>();
		
		petr = gm->addNewFigure(new Human("human/petr.manek@jabbim.com", "CiTrus", "petr.manek@jabbim.com"))
				->setWeaponName("powerful hands of administrator")
				->setSingleRelation("special-th", th)
				->save().unsafeCast<Human>();
		
		gm->addNewFigure(new Human("human/luxor@eideo.cz", "luxor@eideo.cz", "luxor@eideo.cz"));
		// TODO - zvetsit batohy
	}

	void WorldCreator::initTemplates() {
		Location* templateRoom = createObject<Location>("room/templateRoom")
				->setName("Template storage")
				->setDescription("Just a room to keep all the templates at one place. ")
				->save().unsafeCast<Location>();
		
		templates["potion/greenhealing"] = createObject<Potion>("template/potion/greenhealing/1", templateRoom)
				->setType(Potion::PotionType::Healing)
				->setStrength(100)
				->setName("Green potion")
				->setLongName("A green potion in gold vial.")
				->setDescription("The vial is untouched, and the color of the liquid is some kind of scary. ")
				->save();
		
		templates["item/trianglekey"] = createObject<Item>("template/item/trianglekey/1", templateRoom)
				->setName("Triangular key")
				->setLongName("A small triangular key.")
				->setDescription("The key seems rather strange, maybe it unlocks something. ")
				->save();
		
		templates["creature/smallspider"] = createObject<Creature>("template/creature/smallspider/1", templateRoom)
				->drops(templates["potion/greenhealing"], 50 * Dropper::Percent)
				->drops(templates["potion/greenhealing"], 30 * Dropper::Percent)
				->drops(templates["potion/greenhealing"], 1  * Dropper::Percent)
				->setAttack(3)
				->setDefense(2)
				->setMaxHp(100)
				->setCurrentHp(100)
				->setRespawnInterval(90)
				->setWeaponName("poisoned bite")
				->setName("Small spider")
				->setLongName("A small brown spider")
				->setDescription("On closer look, it is cute. But I won't touch it!")
				->save();
	}


	void WorldCreator::bigBang() {
		/**
		 * Base
		 */
		Location* baseRoom = createObject<Location>("room/baseRoom")
				->setRespawnable(true)
				->setName("Base Camp")
				->setDescription("Nothing much to be found here, but get familiar with basic commands - try to type 'help'. By staying here, you are continuously healed a bit.")
				->save().unsafeCast<Location>();
		
		createObject<Healing>("trap/healing/baseRoom")
				->setRate(100 * Healing::PerHour)
				->setTarget(baseRoom)
				->save();

		initAdmins();
		initTemplates();

		AttackTrap* autoAttack = createObject<AttackTrap>("trap/attack");

		/*
		 ************************     Equip room		************************
		 */

		Location* equipRoom = createObject<Location>("room/equipRoom")
				->setName("Equip room")
				->setDescription("Looks like noone has been there for a long time. ")
				->save().unsafeCast<Location>();

			createDoor("equip-base", baseRoom, equipRoom)
					->setGoThroughMessage("You've gone through that door. It made a funny noise. ")
					->setName("wooden door")
					->setLongName("wooden door with simple metal handle")
					->save();

			ObjectPointer potionChest = createObject<Location>("chest/equipRoom/1", equipRoom)
					->setName("Small chest")
					->setDescription("An old, rusty chest. ")
					->save();

				createObject<Potion>("potion/blue/" + RANDID, potionChest)
						->setType(Potion::PotionType::Poison)
						->setStrength(100)
						->setName("Blue potion")
						->setLongName("a blue potion in glass vial")
						->setDescription("It may look like water for some people. ")
						->save();

				createObject<Potion>("potion/redhealing/" + RANDID, potionChest)
						->setType(Potion::PotionType::Healing)
						->setStrength(200)
						->setName("Red potion")
						->setLongName("a red potion in silver vial")
						->setDescription("The vial has some scratches, but the color of the liquid is just beautiful. ")
						->save();
                                
                                createObject<Resource>("resource/gold/" + RANDID, potionChest)
                                                ->setType(Resource::ResourceType::Gold)
                                                ->setQuantity(42)
                                                ->attachSumTrap()
                                                ->save();
                                
                                createObject<Resource>("resource/food/" + RANDID, potionChest)
                                                ->setType(Resource::ResourceType::Food)
                                                ->setQuantity(100)
                                                ->attachSumTrap()
                                                ->save();

			ObjectPointer equipChest = createObject<Location>("chest/equipRoom/2", equipRoom)
					->setName("Big chest")
					->setDescription("An old, rusty chest. ")
					->save();
			
			shallowCloneTemplate(templates["item/trianglekey"], equipChest).unsafeCast<Item>()->respawnEvery(60)->save();
			
			createObject<Crafter>("crafter/anvil/" + RANDID, equipRoom)
					->addRecipe(createObject<Recipe>("recipe/woodenclub/" + RANDID)
						->setExperience(10)
						->setLevel(1)
						->setResource(Resource::Wood, 5)
						->setGoodItem(createObject<Wearable>("recipeitem/woodenclub1/" + RANDID)
							->setAttackBonus(6)
							->setSlot(Wearable::Weapon)
							->setSize(3000)
							->setWeight(5000)
							->setName("Wooden club")
							->setLongName("a weak wooden club")
							->setDescription("It looks like a candy next to a sword, but still better than fighting with lollypop. ")
							->save())
						->setBadItem(createObject<Wearable>("recipeitem/woodenclub1/" + RANDID)
							->setAttackBonus(3)
							->setSlot(Wearable::Weapon)
							->setSize(3000)
							->setWeight(5000)
							->setName("Broken Wooden club")
							->setLongName("a weak cracked wooden club")
							->setDescription("It looks like a candy next to a sword, but still better than fighting with lollypop. ")
							->save())
						->setName("Wooden club")
						->setLongName("Wooden club"))
					->setName("Old anvil")
					->setDescription("An old rusty anvil. ")
					->save();
			
			createObject<Resource>("resource/wood/" + RANDID, equipChest)
						->setType(Resource::ResourceType::Wood)
						->setQuantity(200)
						->attachSumTrap()
						->save();

				createObject<Inventory>("inventory/hardleatherbackpack/" + RANDID, equipChest)
						->setMaxSpace(30000)
						->setMaxWeight(50000)
						->setSlot(Wearable::Backpack)
						->setDefenseBonus(2)
						->setSize(5000)
						->setWeight(2500)
						->setName("Hard leather backpack")
						->setLongName("a more solid leather backpack")
						->save();

				createObject<Wearable>("wearable/woodenclub/" + RANDID, equipChest)
						->setAttackBonus(6)
						->setSlot(Wearable::Weapon)
						->setSize(3000)
						->setWeight(5000)
						->respawnEvery(60)
						->setName("Wooden club")
						->setLongName("a weak wooden club")
						->setDescription("It looks like a candy next to a sword, but still better than fighting with lollypop. ")
						->save();
                                
				createObject<Resource>("resource/gold/" + RANDID, equipChest)
								->setType(Resource::ResourceType::Gold)
								->setQuantity(200)
								->attachSumTrap()
								->save();

			ObjectPointer trapChest = createObject<Location>("chest/equipRoom/3", equipRoom)
					->setEmptyMessage("There are some nails, but they're too small for your fingers to pick. ")
					->setName("Toolbox")
					->setDescription("Looks dangerously. ")
					->attachTrap(createObject<SimpleDamageTrap>("trap/simpledamage/nail")
						->setDamage(42)
						->setJustOnce()
						->setDamageMessage("A nail has fallen and caused you 42 damage. ")
						->save(), "explore")
					->save();


		/*
		 ************************     Dark room		************************
		 */

		Location* darkRoom = createObject<Location>("room/darkRoom")
				->setName("Dark room")
				->setDescription("a scary room with a lack of light.")
				->attachTrap(autoAttack, "inside")
				->save().unsafeCast<Location>();
		
		deepCloneTemplate(templates["creature/smallspider"], darkRoom);
		deepCloneTemplate(templates["creature/smallspider"], darkRoom);

		this->createDoor("dark-base", baseRoom, darkRoom)
				->setGoThroughMessage("You've crawled through that tunnel. It smelled bad. ")
				->addLock(createObject<DoorLock>("doorlock/dark-base-lock/1")
						->setKey(templates["item/trianglekey"]))
				->setName("narrow tunnel")
				->setLongName("a narrow tunnel with no visible ending")
				->save();

		/*
		 ************************     Boss room		************************
		 */
		Location* bossRoom = createObject<Location>("room/bossRoom")
				->setName("Round room")
				->setDescription("Prepare for the fight!")
				->save().unsafeCast<Location>();

		this->createDoor("dark-boss", bossRoom, darkRoom)
				->setGoThroughMessage("You can't get rid of that strange feeling. ")
				->setName("Crevice")
				->setLongName("hole just big enough to fit in")
				->save();
		
		 createObject<Creature>("creature/megaspider/" + RANDID, bossRoom)
				->drops(createObject<Wearable>("template/wearable/ironclub/1")
							->setAttackBonus(20)
							->setDefenseBonus(3)
							->setSlot(Wearable::Weapon)
							->setSize(5000)
							->setWeight(10000)
							->setName("Iron club")
							->setLongName("a strong iron club")
							->setDescription("Finally a weapon!")
							->save(), 80 * Dropper::Percent)
				->setAttack(8)
				->setDefense(4)
				->setMaxHp(300)
				->setCurrentHp(300)
				->setRespawnInterval(9000)
				->setWeaponName("ugly thorny leg")
				->setName("Big spider")
				->setLongName("A big ugly black spider")
				->setDescription("You would even look closer?!")
				->save();
	}

	Door* WorldCreator::createDoor(string name, Location* a, Location* b, bool two_way) {
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

