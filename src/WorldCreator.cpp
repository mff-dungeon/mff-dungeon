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
				->setDescription("Just a room to keep all the templates at one place.")
				->save().unsafeCast<Location>();
		
		/***********************************************************************
		 *			Resources templates for droppers
		 **********************************************************************/
		
		templates["resource/wood"] = createObject<Resource>("template/resource/wood/1", templateRoom)
				->setType(Resource::ResourceType::Wood)
				->setQuantity(1)
				->save();
		templates["resource/iron"] = createObject<Resource>("template/resource/iron/1", templateRoom)
				->setType(Resource::ResourceType::Iron)
				->setQuantity(1)
				->save();
		templates["resource/dragonskin"] = createObject<Resource>("template/resource/dragonskin/1", templateRoom)
				->setType(Resource::ResourceType::DragonSkin)
				->setQuantity(1)
				->save();
		templates["resource/leather"] = createObject<Resource>("template/resource/leather/1", templateRoom)
				->setType(Resource::ResourceType::Leather)
				->setQuantity(1)
				->save();
		templates["resource/sand"] = createObject<Resource>("template/resource/sand/1", templateRoom)
				->setType(Resource::ResourceType::Sand)
				->setQuantity(1)
				->save();
		templates["resource/whitepowder"] = createObject<Resource>("template/resource/whitepowder/1", templateRoom)
				->setType(Resource::ResourceType::WhitePowder)
				->setQuantity(1)
				->save();
		templates["resource/redpowder"] = createObject<Resource>("template/resource/redpowder/1", templateRoom)
				->setType(Resource::ResourceType::RedPowder)
				->setQuantity(1)
				->save();
		templates["resource/magicalstone"] = createObject<Resource>("template/resource/magicalstone/1", templateRoom)
				->setType(Resource::ResourceType::MagicalStone)
				->setQuantity(1)
				->save();
		
		/**********************************************************************
		 *			Weapons (weak and strong forms)
         **********************************************************************/
		templates["wearable/woodenaxe"] = createObject<Wearable>("template/wearable/woodenaxe/1", templateRoom)
				->setSlot(Wearable::Slot::Weapon)
				->setAttackBonus(6)
				->setSize(3000)
				->setWeight(3500)
				->setName("Wooden axe")
				->setLongName("a wooden one-handed axe")
				->setDescription("...")
				->save();
		templates["wearable/woodenaxe2"] = createObject<Wearable>("template/wearable/woodenaxe2/1", templateRoom)
				->setSlot(Wearable::Slot::Weapon)
				->setAttackBonus(3)
				->setSize(2300)
				->setWeight(2800)
				->setName("Broken wooden axe")
				->setLongName("a broken wooden axe")
				->setDescription("...")
				->save();
		templates["wearable/club"] = createObject<Wearable>("template/wearable/club/1", templateRoom)
				->setSlot(Wearable::Slot::Weapon)
				->setAttackBonus(11)
				->setSize(5000)
				->setWeight(6000)
				->setName("Club")
				->setLongName("a weak wooden club")
				->setDescription("...")
				->save();
		templates["wearable/club2"] = createObject<Wearable>("template/wearable/club2/1", templateRoom)
				->setSlot(Wearable::Slot::Weapon)
				->setAttackBonus(7)
				->setSize(4100)
				->setWeight(5000)
				->setName("Crude club")
				->setLongName("a crude wooden club")
				->setDescription("...")
				->save();
		templates["wearable/irondagger"] = createObject<Wearable>("template/wearable/irondagger/1", templateRoom)
				->setSlot(Wearable::Slot::Weapon)
				->setAttackBonus(15)
				->setSize(250)
				->setWeight(1800)
				->setName("Iron dagger")
				->setLongName("a small iron dagger")
				->setDescription("...")
				->save();
		templates["wearable/irondagger2"] = createObject<Wearable>("template/wearable/irondagger2/1", templateRoom)
				->setSlot(Wearable::Slot::Weapon)
				->setAttackBonus(9)
				->setSize(230)
				->setWeight(1700)
				->setName("Blunt iron dagger")
				->setLongName("a small blunt iron dagger")
				->setDescription("...")
				->save();
		templates["wearable/ironspear"] = createObject<Wearable>("template/wearable/ironspear/1", templateRoom)
				->setSlot(Wearable::Slot::Weapon)
				->setAttackBonus(23)
				->setSize(1000)
				->setWeight(2800)
				->setName("Iron spear")
				->setLongName("a short iron-tipped spear")
				->setDescription("...")
				->save();
		templates["wearable/ironspear2"] = createObject<Wearable>("template/wearable/ironspear2/1", templateRoom)
				->setSlot(Wearable::Slot::Weapon)
				->setAttackBonus(14)
				->setSize(1000)
				->setWeight(2600)
				->setName("Curved iron spear")
				->setLongName("a short and badly curved iron-tipped spear")
				->setDescription("...")
				->save();
		templates["wearable/ironsword"] = createObject<Wearable>("template/wearable/ironsword/1", templateRoom)
				->setSlot(Wearable::Slot::Weapon)
				->setAttackBonus(33)
				->setSize(700)
				->setWeight(5200)
				->setName("Iron sword")
				->setLongName("a fine iron sword")
				->setDescription("...")
				->save();
		templates["wearable/ironsword2"] = createObject<Wearable>("template/wearable/ironsword2/1", templateRoom)
				->setSlot(Wearable::Slot::Weapon)
				->setAttackBonus(19)
				->setSize(700)
				->setWeight(4800)
				->setName("Toothy iron sword")
				->setLongName("a badly-crafted iron sword")
				->setDescription("...")
				->save();
		templates["wearable/ironmace"] = createObject<Wearable>("template/wearable/ironmace/1", templateRoom)
				->setSlot(Wearable::Slot::Weapon)
				->setAttackBonus(41)
				->setSize(1800)
				->setWeight(14000)
				->setName("Iron mace")
				->setLongName("a basic iron mace")
				->setDescription("...")
				->save();
		templates["wearable/ironmace2"] = createObject<Wearable>("template/wearable/ironmace2/1", templateRoom)
				->setSlot(Wearable::Slot::Weapon)
				->setAttackBonus(24)
				->setSize(1800)
				->setWeight(12000)
				->setName("Cracked iron mace")
				->setLongName("a cracked iron mace")
				->setDescription("...")
				->save();
		templates["wearable/ironhammer"] = createObject<Wearable>("template/wearable/ironhammer/1", templateRoom)
				->setSlot(Wearable::Slot::Weapon)
				->setAttackBonus(55)
				->setSize(3500)
				->setWeight(23000)
				->setName("Iron warhammer")
				->setLongName("a big iron warhammer")
				->setDescription("...")
				->save();
		templates["wearable/ironhammer2"] = createObject<Wearable>("template/wearable/ironhammer2/1", templateRoom)
				->setSlot(Wearable::Slot::Weapon)
				->setAttackBonus(27)
				->setSize(3500)
				->setWeight(20000)
				->setName("Blunt iron warhammer")
				->setLongName("a blunt iron warhammer")
				->setDescription("...")
				->save();
		// Weak version is classic iron spear
		templates["wearable/enironspear"] = createObject<Wearable>("template/wearable/enironspear/1", templateRoom)
				->setSlot(Wearable::Slot::Weapon)
				->setAttackBonus(65)
				->setHpBonus(600)
				->setSize(1000)
				->setWeight(2400)
				->setName("Enhanced iron spear")
				->setLongName("an empowered iron spear")
				->setDescription("...")
				->save();
		// Weak version is classic iron sword
		templates["wearable/enironsword"] = createObject<Wearable>("template/wearable/enironsword/1", templateRoom)
				->setSlot(Wearable::Slot::Weapon)
				->setAttackBonus(75)
				->setHpBonus(300)
				->setSize(700)
				->setWeight(4600)
				->setName("Enhanced iron sword")
				->setLongName("a magical iron sword")
				->setDescription("...")
				->save();
		// Weak version is iron spear
		templates["wearable/steelspear"] = createObject<Wearable>("template/wearable/steelspear/1", templateRoom)
				->setSlot(Wearable::Slot::Weapon)
				->setAttackBonus(86)
				->setSize(1100)
				->setWeight(2900)
				->setName("Steel spear")
				->setLongName("a steel tipped spear")
				->setDescription("...")
				->save();
		// Weak version is iron sword
		templates["wearable/steelsword"] = createObject<Wearable>("template/wearable/steelsword/1", templateRoom)
				->setSlot(Wearable::Slot::Weapon)
				->setAttackBonus(99)
				->setSize(800)
				->setWeight(5700)
				->setName("Steel sword")
				->setLongName("a strong steel sword")
				->setDescription("...")
				->save();
		// Weak version is iron mace
		templates["wearable/steelmace"] = createObject<Wearable>("template/wearable/steelmace/1", templateRoom)
				->setSlot(Wearable::Slot::Weapon)
				->setAttackBonus(120)
				->setSize(1700)
				->setWeight(13000)
				->setName("Steel Mace")
				->setLongName("a sturdy steel mace")
				->setDescription("...")
				->save();
		// Weak version is iron warhammer
		templates["wearable/steelwarhammer"] = createObject<Wearable>("template/wearable/steelwarhammer/1", templateRoom)
				->setSlot(Wearable::Slot::Weapon)
				->setAttackBonus(160)
				->setSize(3700)
				->setWeight(20800)
				->setName("Steel warhammer")
				->setLongName("a powerful steel warhammer")
				->setDescription("...")
				->save();
		// Weak version is steel spear
		templates["wearable/ensteelspear"] = createObject<Wearable>("template/wearable/ensteelspear/1", templateRoom)
				->setSlot(Wearable::Slot::Weapon)
				->setAttackBonus(166)
				->setHpBonus(1000)
				->setSize(1100)
				->setWeight(2400)
				->setName("Enhanced steel spear")
				->setLongName("an empowered steel spear")
				->setDescription("...")
				->save();
		// Weak version is steel sword
		templates["wearable/ensteelsword"] = createObject<Wearable>("template/wearable/ensteelsword/1", templateRoom)
				->setSlot(Wearable::Slot::Weapon)
				->setAttackBonus(183)
				->setHpBonus(600)
				->setSize(800)
				->setWeight(5200)
				->setName("Enhanced steel sword")
				->setLongName("a very powerful enhanced steel sword")
				->setDescription("...")
				->save();
		/**********************************************************************
		 *			Armor (weak and strong forms)
         **********************************************************************/
		templates["wearable/leatherbody"] = createObject<Wearable>("template/wearable/leatherbody/1", templateRoom)
				->setSlot(Wearable::Slot::BodyArmor)
				->setDefenseBonus(15)
				->setSize(25000)
				->setWeight(10000)
				->setName("Leather body armor")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["wearable/leatherbody2"] = createObject<Wearable>("template/wearable/leatherbody2/1", templateRoom)
				->setSlot(Wearable::Slot::BodyArmor)
				->setDefenseBonus(8)
				->setSize(22000)
				->setWeight(8000)
				->setName("Torn leather body armor")
				->setLongName("...")
				->setDescription("...")
				->save();
		// Weak version is leather body
		templates["wearable/hardleatherbody"] = createObject<Wearable>("template/wearable/hardleatherbody/1", templateRoom)
				->setSlot(Wearable::Slot::BodyArmor)
				->setDefenseBonus(31)
				->setHpBonus(300)
				->setSize(25000)
				->setWeight(20000)
				->setName("Hard leather body armor")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["wearable/ironbody"] = createObject<Wearable>("template/wearable/ironbody/1", templateRoom)
				->setSlot(Wearable::Slot::BodyArmor)
				->setDefenseBonus(56)
				->setHpBonus(600)
				->setSize(40000)
				->setWeight(40000)
				->setName("Iron body armor")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["wearable/ironbody2"] = createObject<Wearable>("template/wearable/ironbody2/1", templateRoom)
				->setSlot(Wearable::Slot::BodyArmor)
				->setDefenseBonus(27)
				->setHpBonus(150)
				->setSize(30000)
				->setWeight(30000)
				->setName("Weak iron body armor")
				->setLongName("...")
				->setDescription("...")
				->save();
		// Weak version is iron body
		templates["wearable/steelbody"] = createObject<Wearable>("template/wearable/steelbody/1", templateRoom)
				->setSlot(Wearable::Slot::BodyArmor)
				->setDefenseBonus(88)
				->setHpBonus(1100)
				->setSize(40000)
				->setWeight(45000)
				->setName("Steel body armor")
				->setLongName("...")
				->setDescription("...")
				->save();
		// Weak version is hard leather body
		templates["wearable/dragonbody"] = createObject<Wearable>("template/wearable/dragonbody/1", templateRoom)
				->setSlot(Wearable::Slot::BodyArmor)
				->setDefenseBonus(131)
				->setAttackBonus(11)
				->setHpBonus(2000)
				->setSize(25000)
				->setWeight(30000)
				->setName("Dragon leather body armor")
				->setLongName("...")
				->setDescription("...")
				->save();
		////////////////////////////////////////////////////////////////////////
		templates["wearable/leatherhelm"] = createObject<Wearable>("template/wearable/leatherhelm/1", templateRoom)
				->setSlot(Wearable::Slot::Helmet)
				->setDefenseBonus(6)
				->setSize(6000)
				->setWeight(2000)
				->setName("Leather helmet")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["wearable/leatherhelm2"] = createObject<Wearable>("template/wearable/leatherhelm2/1", templateRoom)
				->setSlot(Wearable::Slot::Helmet)
				->setDefenseBonus(4)
				->setSize(5500)
				->setWeight(1800)
				->setName("Torn leather helmet")
				->setLongName("...")
				->setDescription("...")
				->save();
		// Weak version is leather helmet
		templates["wearable/hardleatherhelm"] = createObject<Wearable>("template/wearable/hardleatherhelm/1", templateRoom)
				->setSlot(Wearable::Slot::Helmet)
				->setDefenseBonus(10)
				->setSize(6000)
				->setWeight(4000)
				->setName("Hard leather helmet")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["wearable/ironhelm"] = createObject<Wearable>("template/wearable/ironhelm/1", templateRoom)
				->setSlot(Wearable::Slot::Helmet)
				->setDefenseBonus(18)
				->setHpBonus(250)
				->setSize(15000)
				->setWeight(6000)
				->setName("Iron helm")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["wearable/ironhelm2"] = createObject<Wearable>("template/wearable/ironhelm2/1", templateRoom)
				->setSlot(Wearable::Slot::Helmet)
				->setDefenseBonus(9)
				->setHpBonus(100)
				->setSize(15000)
				->setWeight(4500)
				->setName("Cracked iron helm")
				->setLongName("...")
				->setDescription("...")
				->save();
		// Weak version is iron helmet
		templates["wearable/steelhelm"] = createObject<Wearable>("template/wearable/steelhelm/1", templateRoom)
				->setSlot(Wearable::Slot::Helmet)
				->setDefenseBonus(27)
				->setHpBonus(450)
				->setSize(15000)
				->setWeight(8000)
				->setName("Steel helm")
				->setLongName("...")
				->setDescription("...")
				->save();
		// Weak version is hard leather helmet
		templates["wearable/dragonhelm"] = createObject<Wearable>("template/wearable/dragonhelm/1", templateRoom)
				->setSlot(Wearable::Slot::Helmet)
				->setDefenseBonus(46)
				->setAttackBonus(8)
				->setHpBonus(800)
				->setSize(6000)
				->setWeight(5000)
				->setName("Dragon leather helmet")
				->setLongName("...")
				->setDescription("...")
				->save();
		////////////////////////////////////////////////////////////////////////
		templates["wearable/leathergloves"] = createObject<Wearable>("template/wearable/leathergloves/1", templateRoom)
				->setSlot(Wearable::Slot::Gloves)
				->setDefenseBonus(5)
				->setSize(2000)
				->setWeight(700)
				->setName("Leather gloves")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["wearable/leathergloves2"] = createObject<Wearable>("template/wearable/leathergloves2/1", templateRoom)
				->setSlot(Wearable::Slot::Gloves)
				->setDefenseBonus(3)
				->setSize(1700)
				->setWeight(600)
				->setName("Torn leather gloves")
				->setLongName("...")
				->setDescription("...")
				->save();
		// Weak version is leather gloves
		templates["wearable/hardleathergloves"] = createObject<Wearable>("template/wearable/hardleathergloves/1", templateRoom)
				->setSlot(Wearable::Slot::Gloves)
				->setDefenseBonus(8)
				->setSize(2000)
				->setWeight(1300)
				->setName("Hard leather gloves")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["wearable/irongloves"] = createObject<Wearable>("template/wearable/irongloves/1", templateRoom)
				->setSlot(Wearable::Slot::Gloves)
				->setDefenseBonus(14)
				->setHpBonus(120)
				->setSize(5000)
				->setWeight(3000)
				->setName("Iron gauntlets")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["wearable/irongloves2"] = createObject<Wearable>("template/wearable/irongloves2/1", templateRoom)
				->setSlot(Wearable::Slot::Gloves)
				->setDefenseBonus(7)
				->setHpBonus(40)
				->setSize(4000)
				->setWeight(2000)
				->setName("Broken iron gauntlets")
				->setLongName("...")
				->setDescription("...")
				->save();
		// Weak version is iron gauntlets
		templates["wearable/steelgloves"] = createObject<Wearable>("template/wearable/steelgloves/1", templateRoom)
				->setSlot(Wearable::Slot::Gloves)
				->setDefenseBonus(21)
				->setHpBonus(230)
				->setSize(5000)
				->setWeight(3500)
				->setName("Steel gauntlets")
				->setLongName("...")
				->setDescription("...")
				->save();
		// Weak version is hard leather gloves
		templates["wearable/dragongloves"] = createObject<Wearable>("template/wearable/dragongloves/1", templateRoom)
				->setSlot(Wearable::Slot::Gloves)
				->setDefenseBonus(34)
				->setAttackBonus(6)
				->setHpBonus(550)
				->setSize(2000)
				->setWeight(1800)
				->setName("Dragon leather gloves")
				->setLongName("...")
				->setDescription("...")
				->save();
		////////////////////////////////////////////////////////////////////////
		templates["wearable/leatherboots"] = createObject<Wearable>("template/wearable/leatherboots/1", templateRoom)
				->setSlot(Wearable::Slot::Boots)
				->setDefenseBonus(5)
				->setSize(2600)
				->setWeight(1000)
				->setName("Leather boots")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["wearable/leatherboots2"] = createObject<Wearable>("template/wearable/leatherboots2/1", templateRoom)
				->setSlot(Wearable::Slot::Boots)
				->setDefenseBonus(3)
				->setSize(2400)
				->setWeight(900)
				->setName("Torn leather boots")
				->setLongName("...")
				->setDescription("...")
				->save();
		// Weak version is leather boots
		templates["wearable/hardleatherboots"] = createObject<Wearable>("template/wearable/hardleatherboots/1", templateRoom)
				->setSlot(Wearable::Slot::Boots)
				->setDefenseBonus(9)
				->setSize(2600)
				->setWeight(1700)
				->setName("Hard leather boots")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["wearable/ironboots"] = createObject<Wearable>("template/wearable/ironboots/1", templateRoom)
				->setSlot(Wearable::Slot::Boots)
				->setDefenseBonus(15)
				->setHpBonus(120)
				->setSize(6000)
				->setWeight(4000)
				->setName("Iron boots")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["wearable/ironboots2"] = createObject<Wearable>("template/wearable/ironboots2/1", templateRoom)
				->setSlot(Wearable::Slot::Boots)
				->setDefenseBonus(8)
				->setHpBonus(120)
				->setSize(5500)
				->setWeight(4000)
				->setName("Broken iron boots")
				->setLongName("...")
				->setDescription("...")
				->save();
		// Weak version is iron boots
		templates["wearable/steelboots"] = createObject<Wearable>("template/wearable/steelboots/1", templateRoom)
				->setSlot(Wearable::Slot::Boots)
				->setDefenseBonus(24)
				->setHpBonus(230)
				->setSize(6000)
				->setWeight(4800)
				->setName("Steel boots")
				->setLongName("...")
				->setDescription("...")
				->save();
		// Weak version is hard leather boots
		templates["wearable/dragonboots"] = createObject<Wearable>("template/wearable/dragonboots/1", templateRoom)
				->setSlot(Wearable::Slot::Boots)
				->setDefenseBonus(38)
				->setAttackBonus(6)
				->setHpBonus(550)
				->setSize(2600)
				->setWeight(2500)
				->setName("Dragon leather boots")
				->setLongName("...")
				->setDescription("...")
				->save();
		////////////////////////////////////////////////////////////////////////
		templates["wearable/crudewoodshield"] = createObject<Wearable>("template/wearable/crudewoodshield/1", templateRoom)
				->setSlot(Wearable::Slot::Shield)
				->setDefenseBonus(18)
				->setSize(5000)
				->setWeight(6000)
				->setName("Crude wooden shield")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["wearable/crudewoodshield2"] = createObject<Wearable>("template/wearable/crudewoodshield2/1", templateRoom)
				->setSlot(Wearable::Slot::Shield)
				->setDefenseBonus(10)
				->setSize(5000)
				->setWeight(5000)
				->setName("Crude cracked wooden shield")
				->setLongName("...")
				->setDescription("...")
				->save();
		// Weak version is crude wooden shield
		templates["wearable/woodshield"] = createObject<Wearable>("template/wearable/woodshield/1", templateRoom)
				->setSlot(Wearable::Slot::Shield)
				->setDefenseBonus(34)
				->setSize(5000)
				->setWeight(8000)
				->setName("Strong wooden shield")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["wearable/ironshield"] = createObject<Wearable>("template/wearable/ironshield/1", templateRoom)
				->setSlot(Wearable::Slot::Shield)
				->setDefenseBonus(64)
				->setSize(6000)
				->setWeight(20000)
				->setName("Iron shield")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["wearable/ironshield2"] = createObject<Wearable>("template/wearable/ironshield2/1", templateRoom)
				->setSlot(Wearable::Slot::Shield)
				->setDefenseBonus(37)
				->setSize(6000)
				->setWeight(17000)
				->setName("Cracked iron shield")
				->setLongName("...")
				->setDescription("...")
				->save();
		// Weak version is iron shield
		templates["wearable/steelshield"] = createObject<Wearable>("template/wearable/steelshield/1", templateRoom)
				->setSlot(Wearable::Slot::Shield)
				->setDefenseBonus(102)
				->setSize(7000)
				->setWeight(25000)
				->setName("Steel shield")
				->setLongName("...")
				->setDescription("...")
				->save();
		// Weak version is steel shield
		templates["wearable/ensteelshield"] = createObject<Wearable>("template/wearable/ensteelshield/1", templateRoom)
				->setSlot(Wearable::Slot::Shield)
				->setDefenseBonus(152)
				->setSize(7000)
				->setWeight(20000)
				->setName("Enhanced steel shield")
				->setLongName("...")
				->setDescription("...")
				->save();
		
		/**********************************************************************
		 *			Backpacks (weak and strong forms)
         **********************************************************************/
		templates["inventory/leatherpack"] = createObject<Inventory>("template/inventory/leatherpack/1", templateRoom)
				->setMaxSpace(10000)
				->setMaxWeight(10000)
				->setBaseSize(3000)
				->setBaseWeight(2300)
				->setSlot(Wearable::Backpack)
				->setName("Leather backpack")
				->setLongName("...")
				->save();
		templates["inventory/leatherpack2"] = createObject<Inventory>("template/inventory/leatherpack2/1", templateRoom)
				->setMaxSpace(6000)
				->setMaxWeight(6000)
				->setBaseSize(2000)
				->setBaseWeight(1500)
				->setSlot(Wearable::Backpack)
				->setName("Torn leather backpack")
				->setLongName("...")
				->save();
		// Weak version is leather backpack
		templates["inventory/bigleatherpack"] = createObject<Inventory>("template/inventory/bigleatherpack/1", templateRoom)
				->setMaxSpace(30000)
				->setMaxWeight(30000)
				->setBaseSize(6000)
				->setBaseWeight(4600)
				->setSlot(Wearable::Backpack)
				->setDefenseBonus(4)
				->setName("Big leather backpack")
				->setLongName("...")
				->save();
		// Weak version is big leather backpack
		templates["inventory/hardleatherpack"] = createObject<Inventory>("template/inventory/hardleatherpack/1", templateRoom)
				->setMaxSpace(30000)
				->setMaxWeight(65000)
				->setBaseSize(7500)
				->setBaseWeight(8000)
				->setSlot(Wearable::Backpack)
				->setDefenseBonus(9)
				->setName("Hard leather backpack")
				->setLongName("...")
				->save();
		// Weak version is hard leather backpack
		templates["inventory/dragonleatherpack"] = createObject<Inventory>("template/inventory/dragonleatherpack/1", templateRoom)
				->setMaxSpace(55000)
				->setMaxWeight(120000)
				->setBaseSize(8800)
				->setBaseWeight(13200)
				->setSlot(Wearable::Backpack)
				->setDefenseBonus(21)
				->setName("Dragon leather backpack")
				->setLongName("...")
				->save();
		// Weak version is dragon leather backpack
		templates["inventory/endragonleatherpack"] = createObject<Inventory>("template/inventory/endragonleatherpack/1", templateRoom)
				->setMaxSpace(100000)
				->setMaxWeight(230000)
				->setBaseSize(9600)
				->setBaseWeight(14400)
				->setSlot(Wearable::Backpack)
				->setDefenseBonus(26)
				->setHpBonus(10000)
				->setName("Magical Dragon leather backpack")
				->setLongName("...")
				->save();
		/**********************************************************************
		 *			Potions (weak and strong forms)
         **********************************************************************/
		// Result of tiny fail
		templates["potion/tinylightred"] = createObject<Potion>("template/potion/tinylightred/1", templateRoom)
				->setType(Potion::PotionType::Healing)
				->setStrength(40)
				->setName("Tiny light red potion")
				->setLongName("a faint red potion in tiny vial")
				->setDescription("...")
				->save();
		// Weak version is tiny light red
		templates["potion/tinyred"] = createObject<Potion>("template/potion/tinyred/1", templateRoom)
				->setType(Potion::PotionType::Healing)
				->setStrength(120)
				->setName("Tiny red potion")
				->setLongName("a red potion in a little vial")
				->setDescription("...")
				->save();
		// Weak version is tiny red
		templates["potion/smallred"] = createObject<Potion>("template/potion/smallred/1", templateRoom)
				->setType(Potion::PotionType::Healing)
				->setStrength(330)
				->setName("Small red potion")
				->setLongName("a red potion in a vial")
				->setDescription("...")
				->save();
		// Weak version is small red
		templates["potion/red"] = createObject<Potion>("template/potion/red/1", templateRoom)
				->setType(Potion::PotionType::Healing)
				->setStrength(740)
				->setName("Red potion")
				->setLongName("a dark red potion in a vial")
				->setDescription("...")
				->save();
		// Weak version is red
		templates["potion/bigred"] = createObject<Potion>("template/potion/bigred/1", templateRoom)
				->setType(Potion::PotionType::Healing)
				->setStrength(1120)
				->setName("Big red potion")
				->setLongName("a red potion in a flask")
				->setDescription("...")
				->save();
		// Weak version is big red
		templates["potion/largered"] = createObject<Potion>("template/potion/largered/1", templateRoom)
				->setType(Potion::PotionType::Healing)
				->setStrength(2100)
				->setName("Large red potion")
				->setLongName("a dark red potion in a flask")
				->setDescription("...")
				->save();
		/**********************************************************************
		 *					Keys 
         **********************************************************************/
		templates["item/spiderkey"] = createObject<Item>("template/item/spiderkey/1", templateRoom)
				->setName("Spider key")
				->setLongName("a key with an imprint of spider")
				->setDescription("The key seems to give access to a spider nest.")
				->save();
		templates["item/unicornkey"] = createObject<Item>("template/item/unicornkey/1", templateRoom)
				->setName("Unicorn key")
				->setLongName("a key with an imprint of unicorn")
				->setDescription("The key seems to give access to a unicorn's forest.")
				->save();
		templates["item/minotaurkey"] = createObject<Item>("template/item/minotaurkey/1", templateRoom)
				->setName("Minotaur key")
				->setLongName("a key with an imprint of minotaur")
				->setDescription("The key seems to give access to a minotaur's lair.")
				->save();
		templates["item/hydrakey"] = createObject<Item>("template/item/hydrakey/1", templateRoom)
				->setName("Hydra key")
				->setLongName("a key with an imprint of hydra")
				->setDescription("The key seems to give access to a hydra's lake.")
				->save();
		templates["item/dragonkey"] = createObject<Item>("template/item/dragonkey/1", templateRoom)
				->setName("Dragon key")
				->setLongName("a key with an imprint of dragon")
				->setDescription("The key seems to give access to a dragon's volcano.")
				->save();
		// Access key - a drop from boss to access some areas of the dungeon
		templates["item/pentagonalkey"] = createObject<Item>("template/item/pentagonalkey/1", templateRoom)
				->setName("Pentagon key")
				->setLongName("a key in a shape of pentagon")
				->setDescription("I'll need this key to access further areas of the dungeon.")
				->save();
		// Late access key - a drop from boss to access some areas of the dungeon
		templates["item/heptagonalkey"] = createObject<Item>("template/item/heptagonalkey/1", templateRoom)
				->setName("Heptagon key")
				->setLongName("a key in a shape of heptagon")
				->setDescription("I'll need this key to access the most dangerous areas of the dungeon.")
				->save();
		/**********************************************************************
		 *					Recipes 
         **********************************************************************/
		templates["recipe/woodenaxe"] = createObject<Recipe>("template/recipe/woodenaxe/1")
				->setExperience(15 * RECIPE_EXP_RATE)
				->setLevel(1)
				->setResource(Resource::Wood, 30 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/woodenaxe"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/woodenaxe2"]))
				->setName("Wooden axe")
				->setLongName("a wooden one-handed axe")
				->save();
		templates["recipe/club"] = createObject<Recipe>("template/recipe/club/1")
				->setExperience(25 * RECIPE_EXP_RATE)
				->setLevel(3)
				->setResource(Resource::Wood, 50 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/club"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/club2"]))
				->setName("Club")
				->setLongName("a weak wooden club")
				->save();
		//........... continue l8r
		/*
		 * OLD STUFF THERE IS, WILL BE REMOVED LATER, NOW MAY BE USED FOR TESTING
         */
		
		templates["potion/greenhealing"] = createObject<Potion>("template/potion/greenhealing/1", templateRoom)
				->setType(Potion::PotionType::Healing)
				->setStrength(100)
				->setName("Green potion")
				->setLongName("A green potion in gold vial.")
				->setDescription("The vial is untouched, and the color of the liquid is some kind of scary.")
				->setLongName("A green potion in gold vial.")
				->setDescription("The vial is untouched, and the color of the liquid is some kind of scary.")
				->save();
		
		templates["item/trianglekey"] = createObject<Item>("template/item/trianglekey/1", templateRoom)
				->setName("Triangular key")
				->setLongName("A small triangular key.")
				->setDescription("The key seems rather strange, maybe it unlocks something.")
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
				->setDescription("Looks like noone has been there for a long time.")
				->save().unsafeCast<Location>();

			createDoor("equip-base", baseRoom, equipRoom)
					->setGoThroughMessage("You've gone through that door. It made a funny noise.")
					->setName("wooden door")
					->setLongName("wooden door with simple metal handle")
					->save();

			ObjectPointer potionChest = createObject<Location>("chest/equipRoom/1", equipRoom)
					->setName("Small chest")
					->setDescription("An old, rusty chest.")
					->save();

				createObject<Potion>("potion/blue/" + RANDID, potionChest)
						->setType(Potion::PotionType::Poison)
						->setStrength(100)
						->setName("Blue potion")
						->setLongName("a blue potion in glass vial")
						->setDescription("It may look like water for some people.")
						->save();

				createObject<Potion>("potion/redhealing/" + RANDID, potionChest)
						->setType(Potion::PotionType::Healing)
						->setStrength(200)
						->setName("Red potion")
						->setLongName("a red potion in silver vial")
						->setDescription("The vial has some scratches, but the color of the liquid is just beautiful.")
						->save();
                                
                                createObject<Resource>("resource/gold/" + RANDID, potionChest)
                                                ->setType(Resource::ResourceType::Gold)
                                                ->setQuantity(42)
                                                ->attachSumTrap()
                                                ->save();

			ObjectPointer equipChest = createObject<Location>("chest/equipRoom/2", equipRoom)
					->setName("Big chest")
					->setDescription("An old, rusty chest.")
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
							->setDescription("It looks like a candy next to a sword, but still better than fighting with lollypop.")
							->save())
						->setBadItem(createObject<Wearable>("recipeitem/woodenclub1/" + RANDID)
							->setAttackBonus(3)
							->setSlot(Wearable::Weapon)
							->setSize(3000)
							->setWeight(5000)
							->setName("Broken Wooden club")
							->setLongName("a weak cracked wooden club")
							->setDescription("It looks like a candy next to a sword, but still better than fighting with lollypop.")
							->save())
						->setName("Wooden club")
						->setLongName("Wooden club"))
					->setName("Old anvil")
					->setDescription("An old rusty anvil.")
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
						->setDescription("It looks like a candy next to a sword, but still better than fighting with lollypop.")
						->save();
                                
				createObject<Resource>("resource/gold/" + RANDID, equipChest)
								->setType(Resource::ResourceType::Gold)
								->setQuantity(200)
								->attachSumTrap()
								->save();

			ObjectPointer trapChest = createObject<Location>("chest/equipRoom/3", equipRoom)
					->setEmptyMessage("There are some nails, but they're too small for your fingers to pick.")
					->setName("Toolbox")
					->setDescription("Looks dangerously.")
					->attachTrap(createObject<SimpleDamageTrap>("trap/simpledamage/nail")
						->setDamage(42)
						->setJustOnce()
						->setDamageMessage("A nail has fallen and caused you 42 damage.")
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
				->setGoThroughMessage("You've crawled through that tunnel. It smelled bad.")
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
				->setGoThroughMessage("You can't get rid of that strange feeling.")
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

