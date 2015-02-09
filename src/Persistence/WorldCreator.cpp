#include "WorldCreator.hpp"

namespace Dungeon {

	WorldCreator::WorldCreator(GameManager* gm) {
		this->gm = gm;
	}

	void WorldCreator::initAdmins() {
		ThorsHammer* th;
		gm->insertObject(th = new ThorsHammer());
		gm->addNewFigure(new Human("human/aearsis@eideo.cz", "Aearsis", "aearsis@eideo.cz"))
				->setWeaponName("powerful hands of administrator")
				->setSingleRelation("special-th", th)
				->save().unsafeCast<Human>();
		
		gm->addNewFigure(new Human("human/asaru@jabbim.cz", "Salmelu", "asaru@jabbim.cz"))
				->setWeaponName("powerful hands of administrator")
				->setSingleRelation("special-th", th)
				->save().unsafeCast<Human>();
		
		gm->addNewFigure(new Human("human/petr.manek@jabbim.com", "CiTrus", "petr.manek@jabbim.com"))
				->setWeaponName("powerful hands of administrator")
				->setSingleRelation("special-th", th)
				->save().unsafeCast<Human>();
	}

	void WorldCreator::initTemplates() {
		Location* templateRoom = createObject<Location>("room/templateRoom")
				->setName("Template storage")
				->setDescription("Just a room to keep all the templates at one place.")
				->save().unsafeCast<Location>();
		LOG("WorldCreator") << "Template room created. " << LOGF;
		
		/***********************************************************************
		 *			Resources templates for droppers
		 **********************************************************************/
		
		templates["resource/wood"] = createObject<Resource>("template/resource/wood/1", templateRoom)
				->setType(Resource::ResourceType::Wood)
				->setQuantity(1)
				->setWeight(1)
				->setSize(2)
				->save();
		templates["resource/iron"] = createObject<Resource>("template/resource/iron/1", templateRoom)
				->setType(Resource::ResourceType::Iron)
				->setQuantity(1)
				->setWeight(7)
				->setSize(1)
				->save();
		templates["resource/dragonskin"] = createObject<Resource>("template/resource/dragonskin/1", templateRoom)
				->setType(Resource::ResourceType::DragonSkin)
				->setQuantity(1)
				->setWeight(2)
				->setSize(2)
				->save();
		templates["resource/leather"] = createObject<Resource>("template/resource/leather/1", templateRoom)
				->setType(Resource::ResourceType::Leather)
				->setQuantity(1)
				->setWeight(1)
				->setSize(2)
				->save();
		templates["resource/sand"] = createObject<Resource>("template/resource/sand/1", templateRoom)
				->setType(Resource::ResourceType::Sand)
				->setQuantity(1)
				->setWeight(1)
				->setSize(1)
				->save();
		templates["resource/whitepowder"] = createObject<Resource>("template/resource/whitepowder/1", templateRoom)
				->setType(Resource::ResourceType::WhitePowder)
				->setQuantity(1)
				->setWeight(1)
				->setSize(1)
				->save();
		templates["resource/redpowder"] = createObject<Resource>("template/resource/redpowder/1", templateRoom)
				->setType(Resource::ResourceType::RedPowder)
				->setQuantity(1)
				->setWeight(1)
				->setSize(1)
				->save();
		templates["resource/magicalstone"] = createObject<Resource>("template/resource/magicalstone/1", templateRoom)
				->setType(Resource::ResourceType::MagicalStone)
				->setQuantity(1)
				->setWeight(3)
				->setSize(10)
				->save();
		LOG("WorldCreator") << "Resource templates created. " << LOGF;
		
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
				->setDescription("Maybe it could even kill something.")
				->save();
		templates["wearable/woodenaxe2"] = createObject<Wearable>("template/wearable/woodenaxe2/1", templateRoom)
				->setSlot(Wearable::Slot::Weapon)
				->setAttackBonus(3)
				->setSize(2300)
				->setWeight(2800)
				->setName("Broken wooden axe")
				->setLongName("a broken wooden axe")
				->setDescription("It is cracked, but I can still use it to attack someone.")
				->save();
		templates["wearable/club"] = createObject<Wearable>("template/wearable/club/1", templateRoom)
				->setSlot(Wearable::Slot::Weapon)
				->setAttackBonus(11)
				->setSize(5000)
				->setWeight(6000)
				->setName("Club")
				->setLongName("a weak wooden club")
				->setDescription("A chunk of wood. With enough strength, I will kill somebody.")
				->save();
		templates["wearable/club2"] = createObject<Wearable>("template/wearable/club2/1", templateRoom)
				->setSlot(Wearable::Slot::Weapon)
				->setAttackBonus(7)
				->setSize(4100)
				->setWeight(5000)
				->setName("Crude club")
				->setLongName("a crude wooden club")
				->setDescription("Looks like a strong plank, well, better than bare hands.")
				->save();
		templates["wearable/irondagger"] = createObject<Wearable>("template/wearable/irondagger/1", templateRoom)
				->setSlot(Wearable::Slot::Weapon)
				->setAttackBonus(15)
				->setSize(250)
				->setWeight(1800)
				->setName("Iron dagger")
				->setLongName("a small iron dagger")
				->setDescription("A dagger made of iron pieces.")
				->save();
		templates["wearable/irondagger2"] = createObject<Wearable>("template/wearable/irondagger2/1", templateRoom)
				->setSlot(Wearable::Slot::Weapon)
				->setAttackBonus(9)
				->setSize(230)
				->setWeight(1700)
				->setName("Blunt iron dagger")
				->setLongName("a small blunt iron dagger")
				->setDescription("Slashing with this would be a stupid idea.")
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
		templates["wearable/steelhammer"] = createObject<Wearable>("template/wearable/steelhammer/1", templateRoom)
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
		LOG("WorldCreator") << "Weapon templates created. " << LOGF;
		/**********************************************************************
		 *			Armor (weak and strong forms)
         **********************************************************************/
		templates["wearable/leatherbody"] = createObject<Wearable>("template/wearable/leatherbody/1", templateRoom)
				->setSlot(Wearable::Slot::BodyArmor)
				->setDefenseBonus(15)
				->setSize(25000)
				->setWeight(10000)
				->setName("Leather body armor")
				->setLongName("a leather body armor made of leather strips")
				->setDescription("...")
				->save();
		templates["wearable/leatherbody2"] = createObject<Wearable>("template/wearable/leatherbody2/1", templateRoom)
				->setSlot(Wearable::Slot::BodyArmor)
				->setDefenseBonus(8)
				->setSize(22000)
				->setWeight(8000)
				->setName("Torn leather body armor")
				->setLongName("a leather body armor with some holes in it")
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
				->setLongName("a small leather backpack")
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
		LOG("WorldCreator") << "Armor templates created. " << LOGF;
		/**********************************************************************
		 *			Potions (weak and strong forms)
         **********************************************************************/
		// Result of tiny fail
		templates["potion/tinylightred"] = createObject<Potion>("template/potion/tinylightred/1", templateRoom)
				->setType(Potion::PotionType::Healing)
				->setStrength(40)
				->setName("Tiny light red potion")
				->setLongName("a faint red potion in tiny vial")
				->setDescription("You think it can heal you, but the color is just weird.")
				->save();
		// Weak version is tiny light red
		templates["potion/tinyred"] = createObject<Potion>("template/potion/tinyred/1", templateRoom)
				->setType(Potion::PotionType::Healing)
				->setStrength(120)
				->setName("Tiny red potion")
				->setLongName("a red potion in a little vial")
				->setDescription("A small healing potion. Yay.")
				->save();
		// Weak version is tiny red
		templates["potion/smallred"] = createObject<Potion>("template/potion/smallred/1", templateRoom)
				->setType(Potion::PotionType::Healing)
				->setStrength(330)
				->setName("Small red potion")
				->setLongName("a red potion in a vial")
				->setDescription("This vial is not that small anymore. I wonder how effective its effect is.")
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
		LOG("WorldCreator") << "Potion templates created. " << LOGF;
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
		LOG("WorldCreator") << "Key templates created. " << LOGF;
		/**********************************************************************
		 *					Recipes - weapons
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
		templates["recipe/irondagger"] = createObject<Recipe>("template/recipe/irondagger/1")
				->setExperience(22 * RECIPE_EXP_RATE)
				->setLevel(6)
				->setResource(Resource::Wood, 10 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::Iron, 15 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/irondagger"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/irondagger2"]))
				->setName("Iron dagger")
				->setLongName("a small iron dagger")
				->save();
		templates["recipe/ironspear"] = createObject<Recipe>("template/recipe/ironspear/1")
				->setExperience(21 * RECIPE_EXP_RATE)
				->setLevel(10)
				->setResource(Resource::Wood, 30 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::Iron, 5 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/ironspear"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/ironspear2"]))
				->setName("Iron spear")
				->setLongName("a short iron-tipped spear")
				->save();
		templates["recipe/ironsword"] = createObject<Recipe>("template/recipe/ironsword/1")
				->setExperience(50 * RECIPE_EXP_RATE)
				->setLevel(13)
				->setResource(Resource::Wood, 23 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::Iron, 35 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/ironsword"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/ironsword2"]))
				->setName("Iron sword")
				->setLongName("a fine iron sword")
				->save();
		templates["recipe/ironmace"] = createObject<Recipe>("template/recipe/ironmace/1")
				->setExperience(76 * RECIPE_EXP_RATE)
				->setLevel(16)
				->setResource(Resource::Wood, 30 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::Iron, 55 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/ironmace"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/ironmace2"]))
				->setName("Iron mace")
				->setLongName("a basic iron mace")
				->save();
		templates["recipe/ironhammer"] = createObject<Recipe>("template/recipe/ironhammer/1")
				->setExperience(138 * RECIPE_EXP_RATE)
				->setLevel(19)
				->setResource(Resource::Wood, 55 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::Iron, 100 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/ironhammer"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/ironhammer2"]))
				->setName("Iron warhammer")
				->setLongName("a big iron warhammer")
				->save();
		templates["recipe/enironspear"] = createObject<Recipe>("template/recipe/enironsword/1")
				->setExperience(86 * RECIPE_EXP_RATE)
				->setLevel(24)
				->setResource(Resource::Wood, 50 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::Iron, 10 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::MagicalStone, 1 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/enironspear"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/ironspear"]))
				->setName("Enhanced iron spear")
				->setLongName("an empowered iron spear")
				->save();
		templates["recipe/enironsword"] = createObject<Recipe>("template/recipe/enironsword/1")
				->setExperience(117 * RECIPE_EXP_RATE)
				->setLevel(30)
				->setResource(Resource::Wood, 35 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::Iron, 45 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::MagicalStone, 1 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/enironsword"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/ironsword"]))
				->setName("Enhanced iron sword")
				->setLongName("a magical iron sword")
				->save();
		templates["recipe/steelspear"] = createObject<Recipe>("template/recipe/steelspear/1")
				->setExperience(44 * RECIPE_EXP_RATE)
				->setLevel(33)
				->setResource(Resource::Wood, 60 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::Iron, 12 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/steelspear"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/ironspear"]))
				->setName("Steel spear")
				->setLongName("a steel-tipped spear")
				->save();
		templates["recipe/steelsword"] = createObject<Recipe>("template/recipe/steelsword/1")
				->setExperience(101 * RECIPE_EXP_RATE)
				->setLevel(38)
				->setResource(Resource::Wood, 70 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::Iron, 60 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/steelsword"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/ironsword"]))
				->setName("Steel sword")
				->setLongName("a strong steel sword")
				->save();
		templates["recipe/steelmace"] = createObject<Recipe>("template/recipe/steelmace/1")
				->setExperience(154 * RECIPE_EXP_RATE)
				->setLevel(43)
				->setResource(Resource::Wood, 110 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::Iron, 90 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/steelmace"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/ironmace"]))
				->setName("Steel mace")
				->setLongName("a sturdy steel mace")
				->save();
		templates["recipe/steelhammer"] = createObject<Recipe>("template/recipe/steelhammer/1")
				->setExperience(255 * RECIPE_EXP_RATE)
				->setLevel(48)
				->setResource(Resource::Wood, 180 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::Iron, 150 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/steelhammer"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/ironhammer"]))
				->setName("Steel warhammer")
				->setLongName("a powerful steel warhammer")
				->save();
		templates["recipe/ensteelspear"] = createObject<Recipe>("template/recipe/ensteelspear/1")
				->setExperience(393 * RECIPE_EXP_RATE)
				->setLevel(54)
				->setResource(Resource::Wood, 120 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::Iron, 30 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::MagicalStone, 6 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/ensteelspear"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/steelspear"]))
				->setName("Enhanced steel spear")
				->setLongName("an empowered steel spear")
				->save();
		templates["recipe/ensteelsword"] = createObject<Recipe>("template/recipe/ensteelsword/1")
				->setExperience(460 * RECIPE_EXP_RATE)
				->setLevel(60)
				->setResource(Resource::Wood, 100 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::Iron, 100 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::MagicalStone, 6 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/ensteelsword"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/steelsword"]))
				->setName("Enhanced steel sword")
				->setLongName("a very powerful enhanced steel sword")
				->save();
		LOG("WorldCreator") << "Weapon recipe templates created. " << LOGF;
		/**********************************************************************
		 *					Recipes - potions
         **********************************************************************/
		templates["recipe/tinyred"] = createObject<Recipe>("template/recipe/tinyred/1")
				->setExperience(2 * RECIPE_EXP_RATE)
				->setLevel(4)
				->setResource(Resource::Sand, 2 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::WhitePowder, 1 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["potion/tinyred"]))
				->setBadItem(shallowCloneTemplate(templates["potion/tinylightred"]))
				->setName("Tiny red potion")
				->setLongName("a red potion in a little vial")
				->save();
		templates["recipe/smallred"] = createObject<Recipe>("template/recipe/smallred/1")
				->setExperience(4 * RECIPE_EXP_RATE)
				->setLevel(11)
				->setResource(Resource::Sand, 5 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::WhitePowder, 3 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["potion/smallred"]))
				->setBadItem(shallowCloneTemplate(templates["potion/tinyred"]))
				->setName("Small red potion")
				->setLongName("a red potion in a vial")
				->save();
		templates["recipe/red"] = createObject<Recipe>("template/recipe/red/1")
				->setExperience(10 * RECIPE_EXP_RATE)
				->setLevel(22)
				->setResource(Resource::Sand, 10 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::WhitePowder, 7 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::RedPowder, 1 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["potion/red"]))
				->setBadItem(shallowCloneTemplate(templates["potion/smallred"]))
				->setName("Red potion")
				->setLongName("a dark red potion in a vial")
				->save();
		templates["recipe/bigred"] = createObject<Recipe>("template/recipe/bigred/1")
				->setExperience(16 * RECIPE_EXP_RATE)
				->setLevel(34)
				->setResource(Resource::Sand, 20 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::WhitePowder, 10 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::RedPowder, 2 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["potion/bigred"]))
				->setBadItem(shallowCloneTemplate(templates["potion/red"]))
				->setName("Big red potion")
				->setLongName("a red potion in a flask")
				->save();
		templates["recipe/largered"] = createObject<Recipe>("template/recipe/largered/1")
				->setExperience(28 * RECIPE_EXP_RATE)
				->setLevel(53)
				->setResource(Resource::Sand, 28 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::WhitePowder, 16 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::RedPowder, 5 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["potion/largered"]))
				->setBadItem(shallowCloneTemplate(templates["potion/bigred"]))
				->setName("Large red potion")
				->setLongName("a dark red potion in a flask")
				->save();
		LOG("WorldCreator") << "Potion recipe templates created. " << LOGF;
		/**********************************************************************
		 *					Recipes - Armours
         **********************************************************************/
		templates["recipe/leatherbody"] = createObject<Recipe>("template/recipe/leatherbody/1")
				->setExperience(23 * RECIPE_EXP_RATE)
				->setLevel(7)
				->setResource(Resource::Leather, 25 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/leatherbody"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/leatherbody2"]))
				->setName("Leather body armor")
				->setLongName("...")
				->save();
		templates["recipe/hardleatherbody"] = createObject<Recipe>("template/recipe/hardleatherbody/1")
				->setExperience(41 * RECIPE_EXP_RATE)
				->setLevel(17)
				->setResource(Resource::Leather, 45 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/hardleatherbody"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/leatherbody"]))
				->setName("Hard leather body armor")
				->setLongName("...")
				->save();
		templates["recipe/ironbody"] = createObject<Recipe>("template/recipe/ironbody/1")
				->setExperience(194 * RECIPE_EXP_RATE)
				->setLevel(31)
				->setResource(Resource::Wood, 80 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::Iron, 140 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/ironbody"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/ironbody2"]))
				->setName("Iron body armor")
				->setLongName("...")
				->save();
		templates["recipe/steelbody"] = createObject<Recipe>("template/recipe/steelbody/1")
				->setExperience(320 * RECIPE_EXP_RATE)
				->setLevel(46)
				->setResource(Resource::Wood, 200 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::Iron, 200 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/steelbody"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/ironbody"]))
				->setName("Steel body armor")
				->setLongName("...")
				->save();
		templates["recipe/dragonbody"] = createObject<Recipe>("template/recipe/dragonbody/1")
				->setExperience(450 * RECIPE_EXP_RATE)
				->setLevel(59)
				->setResource(Resource::DragonSkin, 30 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::MagicalStone, 3 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/dragonbody"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/hardleatherbody"]))
				->setName("Dragon leather body armor")
				->setLongName("...")
				->save();
		templates["recipe/leatherhelm"] = createObject<Recipe>("template/recipe/leatherhelm/1")
				->setExperience(9 * RECIPE_EXP_RATE)
				->setLevel(5)
				->setResource(Resource::Leather, 9 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/leatherhelm"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/leatherhelm2"]))
				->setName("Leather helmet")
				->setLongName("...")
				->save();
		templates["recipe/hardleatherhelm"] = createObject<Recipe>("template/recipe/hardleatherhelm/1")
				->setExperience(15 * RECIPE_EXP_RATE)
				->setLevel(14)
				->setResource(Resource::Leather, 16 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/hardleatherhelm"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/leatherhelm"]))
				->setName("Hard leather helmet")
				->setLongName("...")
				->save();
		templates["recipe/ironhelm"] = createObject<Recipe>("template/recipe/ironhelm/1")
				->setExperience(65 * RECIPE_EXP_RATE)
				->setLevel(28)
				->setResource(Resource::Wood, 20 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::Iron, 50 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/ironhelm"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/ironhelm2"]))
				->setName("Iron helm")
				->setLongName("...")
				->save();
		templates["recipe/steelhelm"] = createObject<Recipe>("template/recipe/steelhelm/1")
				->setExperience(128 * RECIPE_EXP_RATE)
				->setLevel(44)
				->setResource(Resource::Wood, 80 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::Iron, 80 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/steelhelm"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/ironhelm"]))
				->setName("Steel helm")
				->setLongName("...")
				->save();
		templates["recipe/dragonhelm"] = createObject<Recipe>("template/recipe/dragonhelm/1")
				->setExperience(170 * RECIPE_EXP_RATE)
				->setLevel(58)
				->setResource(Resource::DragonSkin, 12 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::MagicalStone, 1 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/dragonhelm"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/hardleatherhelm"]))
				->setName("Dragon leather helmet")
				->setLongName("...")
				->save();
		templates["recipe/leathergloves"] = createObject<Recipe>("template/recipe/leathergloves/1")
				->setExperience(7 * RECIPE_EXP_RATE)
				->setLevel(3)
				->setResource(Resource::Leather, 7 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/leathergloves"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/leathergloves2"]))
				->setName("Leather gloves")
				->setLongName("...")
				->save();
		templates["recipe/hardleathergloves"] = createObject<Recipe>("template/recipe/hardleathergloves/1")
				->setExperience(12 * RECIPE_EXP_RATE)
				->setLevel(12)
				->setResource(Resource::Leather, 13 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/hardleathergloves"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/leathergloves"]))
				->setName("Hard leather gloves")
				->setLongName("...")
				->save();
		templates["recipe/irongloves"] = createObject<Recipe>("template/recipe/irongloves/1")
				->setExperience(52 * RECIPE_EXP_RATE)
				->setLevel(26)
				->setResource(Resource::Wood, 16 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::Iron, 40 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/irongloves"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/irongloves2"]))
				->setName("Iron gauntlets")
				->setLongName("...")
				->save();
		templates["recipe/steelgloves"] = createObject<Recipe>("template/recipe/steelgloves/1")
				->setExperience(112 * RECIPE_EXP_RATE)
				->setLevel(41)
				->setResource(Resource::Wood, 70 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::Iron, 70 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/steelgloves"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/irongloves"]))
				->setName("Steel gauntlets")
				->setLongName("...")
				->save();
		templates["recipe/dragongloves"] = createObject<Recipe>("template/recipe/dragongloves/1")
				->setExperience(130 * RECIPE_EXP_RATE)
				->setLevel(56)
				->setResource(Resource::DragonSkin, 8 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::MagicalStone, 1 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/dragongloves"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/hardleathergloves"]))
				->setName("Dragon leather gloves")
				->setLongName("...")
				->save();
		templates["recipe/leatherboots"] = createObject<Recipe>("template/recipe/leatherboots/1")
				->setExperience(7 * RECIPE_EXP_RATE)
				->setLevel(3)
				->setResource(Resource::Leather, 7 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/leatherboots"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/leatherboots2"]))
				->setName("Leather boots")
				->setLongName("...")
				->save();
		templates["recipe/hardleatherboots"] = createObject<Recipe>("template/recipe/hardleatherboots/1")
				->setExperience(12 * RECIPE_EXP_RATE)
				->setLevel(12)
				->setResource(Resource::Leather, 13 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/hardleatherboots"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/leatherboots"]))
				->setName("Hard leather boots")
				->setLongName("...")
				->save();
		templates["recipe/ironboots"] = createObject<Recipe>("template/recipe/ironboots/1")
				->setExperience(52 * RECIPE_EXP_RATE)
				->setLevel(26)
				->setResource(Resource::Wood, 16 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::Iron, 40 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/ironboots"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/ironboots2"]))
				->setName("Iron boots")
				->setLongName("...")
				->save();
		templates["recipe/steelboots"] = createObject<Recipe>("template/recipe/steelboots/1")
				->setExperience(112 * RECIPE_EXP_RATE)
				->setLevel(41)
				->setResource(Resource::Wood, 70 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::Iron, 70 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/steelboots"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/ironboots"]))
				->setName("Steel boots")
				->setLongName("...")
				->save();
		templates["recipe/dragonboots"] = createObject<Recipe>("template/recipe/dragonboots/1")
				->setExperience(130 * RECIPE_EXP_RATE)
				->setLevel(56)
				->setResource(Resource::DragonSkin, 8 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::MagicalStone, 1 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/dragonboots"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/hardleatherboots"]))
				->setName("Dragon leather boots")
				->setLongName("...")
				->save();
		templates["recipe/crudewoodshield"] = createObject<Recipe>("template/recipe/crudewoodshield/1")
				->setExperience(13 * RECIPE_EXP_RATE)
				->setLevel(2)
				->setResource(Resource::Wood, 25 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/crudewoodshield"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/crudewoodshield2"]))
				->setName("Crude wooden shield")
				->setLongName("...")
				->save();
		templates["recipe/woodshield"] = createObject<Recipe>("template/recipe/woodshield/1")
				->setExperience(28 * RECIPE_EXP_RATE)
				->setLevel(10)
				->setResource(Resource::Wood, 55 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/woodshield"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/crudewoodshield"]))
				->setName("Strong wooden shield")
				->setLongName("...")
				->save();
		templates["recipe/ironshield"] = createObject<Recipe>("template/recipe/ironshield/1")
				->setExperience(70 * RECIPE_EXP_RATE)
				->setLevel(24)
				->setResource(Resource::Wood, 30 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::Iron, 50 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/ironshield"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/ironshield2"]))
				->setName("Iron shield")
				->setLongName("...")
				->save();
		templates["recipe/steelshield"] = createObject<Recipe>("template/recipe/steelshield/1")
				->setExperience(192 * RECIPE_EXP_RATE)
				->setLevel(39)
				->setResource(Resource::Wood, 120 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::Iron, 120 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/steelshield"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/ironshield"]))
				->setName("Steel shield")
				->setLongName("...")
				->save();
		templates["recipe/ensteelshield"] = createObject<Recipe>("template/recipe/ensteelshield/1")
				->setExperience(445 * RECIPE_EXP_RATE)
				->setLevel(55)
				->setResource(Resource::Wood, 250 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::Iron, 200 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::MagicalStone, 2 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["wearable/ensteelshield"]))
				->setBadItem(shallowCloneTemplate(templates["wearable/steelshield"]))
				->setName("Enhanced steel shield")
				->setLongName("...")
				->save();
		templates["recipe/leatherpack"] = createObject<Recipe>("template/recipe/leatherpack/1")
				->setExperience(5 * RECIPE_EXP_RATE)
				->setLevel(5)
				->setResource(Resource::Leather, 5 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["inventory/leatherpack"]))
				->setBadItem(shallowCloneTemplate(templates["inventory/leatherpack2"]))
				->setName("Leather backpack")
				->setLongName("...")
				->save();
		templates["recipe/bigleatherpack"] = createObject<Recipe>("template/recipe/bigleatherpack/1")
				->setExperience(17 * RECIPE_EXP_RATE)
				->setLevel(11)
				->setResource(Resource::Leather, 18 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["inventory/bigleatherpack"]))
				->setBadItem(shallowCloneTemplate(templates["inventory/leatherpack"]))
				->setName("Big leather backpack")
				->setLongName("...")
				->save();
		templates["recipe/hardleatherpack"] = createObject<Recipe>("template/recipe/hardleatherpack/1")
				->setExperience(50 * RECIPE_EXP_RATE)
				->setLevel(27)
				->setResource(Resource::Leather, 55 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["inventory/hardleatherpack"]))
				->setBadItem(shallowCloneTemplate(templates["inventory/bigleatherpack"]))
				->setName("Big leather backpack")
				->setLongName("...")
				->save();
		templates["recipe/dragonleatherpack"] = createObject<Recipe>("template/recipe/hardleatherpack/1")
				->setExperience(350 * RECIPE_EXP_RATE)
				->setLevel(41)
				->setResource(Resource::DragonSkin, 25 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::MagicalStone, 2 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["inventory/dragonleatherpack"]))
				->setBadItem(shallowCloneTemplate(templates["inventory/hardleatherpack"]))
				->setName("Dragon leather backpack")
				->setLongName("...")
				->save();
		templates["recipe/endragonleatherpack"] = createObject<Recipe>("template/recipe/endragonleatherpack/1")
				->setExperience(1050 * RECIPE_EXP_RATE)
				->setLevel(62)
				->setResource(Resource::DragonSkin, 55 * RECIPE_MAT_REQUIRED)
				->setResource(Resource::MagicalStone, 10 * RECIPE_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["inventory/endragonleatherpack"]))
				->setBadItem(shallowCloneTemplate(templates["inventory/dragonleatherpack"]))
				->setName("Magical dragon leather backpack")
				->setLongName("...")
				->save();
		LOG("WorldCreator") << "Armor recipe templates created. " << LOGF;
		/**********************************************************************
		 *					Recipes - Keys
         **********************************************************************/
		templates["recipe/spiderkey"] = createObject<Recipe>("template/recipe/spiderkey/1")
				->setExperience(22 * RECIPE_EXP_RATE)
				->setLevel(23)
				->setResource(Resource::Wood, 60 * RECIPE_KEY_MAT_REQUIRED)
				->setResource(Resource::Iron, 100 * RECIPE_KEY_MAT_REQUIRED)
				->setResource(Resource::WhitePowder, 40 * RECIPE_KEY_MAT_REQUIRED)
				->setResource(Resource::MagicalStone, 1 * RECIPE_KEY_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["item/spiderkey"]))
				->setBadItem(shallowCloneTemplate(templates["item/spiderkey"]))
				->setName("Spider key")
				->setLongName("a key with an imprint of spider")
				->save();
		templates["recipe/unicornkey"] = createObject<Recipe>("template/recipe/unicornkey/1")
				->setExperience(49 * RECIPE_EXP_RATE)
				->setLevel(36)
				->setResource(Resource::Wood, 150 * RECIPE_KEY_MAT_REQUIRED)
				->setResource(Resource::Iron, 200 * RECIPE_KEY_MAT_REQUIRED)
				->setResource(Resource::WhitePowder, 70 * RECIPE_KEY_MAT_REQUIRED)
				->setResource(Resource::MagicalStone, 2 * RECIPE_KEY_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["item/unicornkey"]))
				->setBadItem(shallowCloneTemplate(templates["item/spiderkey"]))
				->setName("Unicorn key")
				->setLongName("a key with an imprint of unicorn")
				->save();
		templates["recipe/minotaurkey"] = createObject<Recipe>("template/recipe/minotaurkey/1")
				->setExperience(88 * RECIPE_EXP_RATE)
				->setLevel(51)
				->setResource(Resource::Wood, 300 * RECIPE_KEY_MAT_REQUIRED)
				->setResource(Resource::Iron, 300 * RECIPE_KEY_MAT_REQUIRED)
				->setResource(Resource::WhitePowder, 110 * RECIPE_KEY_MAT_REQUIRED)
				->setResource(Resource::RedPowder, 15 * RECIPE_KEY_MAT_REQUIRED)
				->setResource(Resource::MagicalStone, 4 * RECIPE_KEY_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["item/minotaurkey"]))
				->setBadItem(shallowCloneTemplate(templates["item/unicornkey"]))
				->setName("Minotaur key")
				->setLongName("a key with an imprint of minotaur")
				->save();
		templates["recipe/hydrakey"] = createObject<Recipe>("template/recipe/hydrakey/1")
				->setExperience(155 * RECIPE_EXP_RATE)
				->setLevel(65)
				->setResource(Resource::Wood, 600 * RECIPE_KEY_MAT_REQUIRED)
				->setResource(Resource::Iron, 500 * RECIPE_KEY_MAT_REQUIRED)
				->setResource(Resource::WhitePowder, 200 * RECIPE_KEY_MAT_REQUIRED)
				->setResource(Resource::RedPowder, 40 * RECIPE_KEY_MAT_REQUIRED)
				->setResource(Resource::MagicalStone, 6 * RECIPE_KEY_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["item/hydrakey"]))
				->setBadItem(shallowCloneTemplate(templates["item/minotaurkey"]))
				->setName("Hydra key")
				->setLongName("a key with an imprint of hydra")
				->save();
		templates["recipe/dragonkey"] = createObject<Recipe>("template/recipe/dragonkey/1")
				->setExperience(257 * RECIPE_EXP_RATE)
				->setLevel(79)
				->setResource(Resource::Wood, 1200 * RECIPE_KEY_MAT_REQUIRED)
				->setResource(Resource::Iron, 800 * RECIPE_KEY_MAT_REQUIRED)
				->setResource(Resource::WhitePowder, 300 * RECIPE_KEY_MAT_REQUIRED)
				->setResource(Resource::RedPowder, 80 * RECIPE_KEY_MAT_REQUIRED)
				->setResource(Resource::MagicalStone, 9 * RECIPE_KEY_MAT_REQUIRED)
				->setGoodItem(shallowCloneTemplate(templates["item/dragonkey"]))
				->setBadItem(shallowCloneTemplate(templates["item/hydrakey"]))
				->setName("Dragon key")
				->setLongName("a key with an imprint of dragon")
				->save();
		LOG("WorldCreator") << "Key recipe templates created. " << LOGF;
		/**********************************************************************
		 *					Crafters
         **********************************************************************/
		templates["crafter/stoneanvil"] = createObject<Crafter>("template/crafter/stoneanvil/1", templateRoom)
				->addRecipe(deepCloneTemplate(templates["recipe/woodenaxe"]))
				->addRecipe(deepCloneTemplate(templates["recipe/club"]))
				->addRecipe(deepCloneTemplate(templates["recipe/irondagger"]))
				->addRecipe(deepCloneTemplate(templates["recipe/ironspear"]))
				->addRecipe(deepCloneTemplate(templates["recipe/ironsword"]))
				->addRecipe(deepCloneTemplate(templates["recipe/crudewoodshield"]))
				->addRecipe(deepCloneTemplate(templates["recipe/woodshield"]))
				->addRecipe(deepCloneTemplate(templates["recipe/spiderkey"]))
				->addRecipe(deepCloneTemplate(templates["recipe/ironbody"]))
				->addRecipe(deepCloneTemplate(templates["recipe/ironhelm"]))
				->addRecipe(deepCloneTemplate(templates["recipe/irongloves"]))
				->addRecipe(deepCloneTemplate(templates["recipe/ironboots"]))
				->setName("Stone anvil")
				->setLongName("a stone anvil")
				->setDescription("...")
				->save();
		templates["crafter/ironanvil"] = createObject<Crafter>("template/crafter/ironanvil/1", templateRoom)
				->addRecipe(deepCloneTemplate(templates["recipe/ironmace"]))
				->addRecipe(deepCloneTemplate(templates["recipe/ironhammer"]))
				->addRecipe(deepCloneTemplate(templates["recipe/steelspear"]))
				->addRecipe(deepCloneTemplate(templates["recipe/steelsword"]))
				->addRecipe(deepCloneTemplate(templates["recipe/steelmace"]))
				->addRecipe(deepCloneTemplate(templates["recipe/steelhammer"]))
				->addRecipe(deepCloneTemplate(templates["recipe/ironshield"]))
				->addRecipe(deepCloneTemplate(templates["recipe/steelshield"]))
				->addRecipe(deepCloneTemplate(templates["recipe/unicornkey"]))
				->addRecipe(deepCloneTemplate(templates["recipe/minotaurkey"]))
				->addRecipe(deepCloneTemplate(templates["recipe/steelbody"]))
				->addRecipe(deepCloneTemplate(templates["recipe/steelhelm"]))
				->addRecipe(deepCloneTemplate(templates["recipe/steelgloves"]))
				->addRecipe(deepCloneTemplate(templates["recipe/steelboots"]))
				->setName("Iron anvil")
				->setLongName("an iron anvil")
				->setDescription("...")
				->save();
		templates["crafter/glowinganvil"] = createObject<Crafter>("template/crafter/glowinganvil/1", templateRoom)
				->addRecipe(deepCloneTemplate(templates["recipe/enironspear"]))
				->addRecipe(deepCloneTemplate(templates["recipe/enironsword"]))
				->addRecipe(deepCloneTemplate(templates["recipe/ensteelspear"]))
				->addRecipe(deepCloneTemplate(templates["recipe/ensteelsword"]))
				->addRecipe(deepCloneTemplate(templates["recipe/ensteelshield"]))
				->addRecipe(deepCloneTemplate(templates["recipe/hydrakey"]))
				->addRecipe(deepCloneTemplate(templates["recipe/dragonkey"]))
				->setName("Glowing anvil")
				->setLongName("a magically glowing anvil")
				->setDescription("...")
				->save();
		templates["crafter/alchemy"] = createObject<Crafter>("template/crafter/alchemy/1", templateRoom)
				->addRecipe(deepCloneTemplate(templates["recipe/tinyred"]))
				->addRecipe(deepCloneTemplate(templates["recipe/smallred"]))
				->addRecipe(deepCloneTemplate(templates["recipe/red"]))
				->addRecipe(deepCloneTemplate(templates["recipe/bigred"]))
				->addRecipe(deepCloneTemplate(templates["recipe/largered"]))
				->setName("Alchemist's table")
				->setLongName("an alchemist's table")
				->setDescription("...")
				->save();
		templates["crafter/tanning"] = createObject<Crafter>("template/crafter/tanning/1", templateRoom)
				->addRecipe(deepCloneTemplate(templates["recipe/leatherbody"]))
				->addRecipe(deepCloneTemplate(templates["recipe/hardleatherbody"]))
				->addRecipe(deepCloneTemplate(templates["recipe/leatherhelm"]))
				->addRecipe(deepCloneTemplate(templates["recipe/hardleatherhelm"]))
				->addRecipe(deepCloneTemplate(templates["recipe/leathergloves"]))
				->addRecipe(deepCloneTemplate(templates["recipe/hardleathergloves"]))
				->addRecipe(deepCloneTemplate(templates["recipe/leatherboots"]))
				->addRecipe(deepCloneTemplate(templates["recipe/hardleatherboots"]))
				->setName("Tanning rack")
				->setLongName("a tanning rack")
				->setDescription("...")
				->save();
		templates["crafter/glowingtanning"] = createObject<Crafter>("template/crafter/glowingtanning/1", templateRoom)
				->addRecipe(deepCloneTemplate(templates["recipe/dragonbody"]))
				->addRecipe(deepCloneTemplate(templates["recipe/dragonhelm"]))
				->addRecipe(deepCloneTemplate(templates["recipe/dragongloves"]))
				->addRecipe(deepCloneTemplate(templates["recipe/dragonboots"]))
				->setName("Glowing tanning rack")
				->setLongName("a glowing tanning rack")
				->setDescription("...")
				->save();
		LOG("WorldCreator") << "Crafters created. " << LOGF;
		/**********************************************************************
		 *					Creatures
         **********************************************************************/
		templates["creature/smallspider"] = createObject<Creature>("template/creature/smallspider/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/wood"]), 32 * Dropper::Percent, 6 * ENEMY_MATS_DROP_RATE, 14 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/sand"]), 23 * Dropper::Percent, 11 * ENEMY_MATS_DROP_RATE, 16 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/whitepowder"]), 19  * Dropper::Percent, 6 * ENEMY_MATS_DROP_RATE, 9 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/tinyred"]), 8  * Dropper::Percent)
				->setAttack(5)
				->setDefense(3)
				->setMaxHp(4 * ENEMY_HP_RATE)
				->setCurrentHp(4 * ENEMY_HP_RATE)
				->setRespawnInterval(12 * ENEMY_RESPAWN_RATE)
				->setWeaponName("poisoned bite")
				->setName("Small spider")
				->setLongName("A small brown spider")
				->setDescription("On closer look, it is cute. But I won't touch it!")
				->save();
		templates["creature/rat"] = createObject<Creature>("template/creature/rat/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/wood"]), 31 * Dropper::Percent, 9 * ENEMY_MATS_DROP_RATE, 16 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/leather"]), 40 * Dropper::Percent, 6 * ENEMY_MATS_DROP_RATE, 10 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/tinyred"]), 9 * Dropper::Percent)
				->setAttack(9)
				->setDefense(5)
				->setMaxHp(5 * ENEMY_HP_RATE)
				->setCurrentHp(5 * ENEMY_HP_RATE)
				->setRespawnInterval(12 * ENEMY_RESPAWN_RATE)
				->setWeaponName("little claws")
				->setName("Rat")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["creature/raven"] = createObject<Creature>("template/creature/raven/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/sand"]), 100 * Dropper::Percent, 11 * ENEMY_MATS_DROP_RATE, 20 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/whitepowder"]), 80  * Dropper::Percent, 1 * ENEMY_MATS_DROP_RATE, 3 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/tinyred"]), 10 * Dropper::Percent)
				->setAttack(12)
				->setDefense(9)
				->setMaxHp(8 * ENEMY_HP_RATE)
				->setCurrentHp(8 * ENEMY_HP_RATE)
				->setRespawnInterval(12 * ENEMY_RESPAWN_RATE)
				->setWeaponName("hungry beak")
				->setName("Raven")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["creature/snake"] = createObject<Creature>("template/creature/snake/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/wood"]), 79 * Dropper::Percent, 3 * ENEMY_MATS_DROP_RATE, 7 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/iron"]), 27 * Dropper::Percent, 4 * ENEMY_MATS_DROP_RATE, 16 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/leather"]), 37 * Dropper::Percent, 7 * ENEMY_MATS_DROP_RATE, 11 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/tinyred"]), 11 * Dropper::Percent)
				->setAttack(16)
				->setDefense(11)
				->setMaxHp(11 * ENEMY_HP_RATE)
				->setCurrentHp(11 * ENEMY_HP_RATE)
				->setRespawnInterval(12 * ENEMY_RESPAWN_RATE)
				->setWeaponName("poisoned bite")
				->setName("Snake")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["creature/bigrat"] = createObject<Creature>("template/creature/bigrat/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/wood"]), 90 * Dropper::Percent, 3 * ENEMY_MATS_DROP_RATE, 6 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/iron"]), 51 * Dropper::Percent, 3 * ENEMY_MATS_DROP_RATE, 4 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/leather"]), 66 * Dropper::Percent, 6 * ENEMY_MATS_DROP_RATE, 14 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/tinyred"]), 12 * Dropper::Percent)
				->setAttack(19)
				->setDefense(16)
				->setMaxHp(11 * ENEMY_HP_RATE)
				->setCurrentHp(11 * ENEMY_HP_RATE)
				->setRespawnInterval(12 * ENEMY_RESPAWN_RATE)
				->setWeaponName("big claws")
				->setName("Overgrown rat")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["creature/scorpion"] = createObject<Creature>("template/creature/scorpion/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/iron"]), 28 * Dropper::Percent, 11 * ENEMY_MATS_DROP_RATE, 21 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/sand"]), 62 * Dropper::Percent, 7 * ENEMY_MATS_DROP_RATE, 14 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/whitepowder"]), 84  * Dropper::Percent, 7 * ENEMY_MATS_DROP_RATE, 13 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/tinyred"]), 13 * Dropper::Percent)
				->setAttack(22)
				->setDefense(19)
				->setMaxHp(20 * ENEMY_HP_RATE)
				->setCurrentHp(20 * ENEMY_HP_RATE)
				->setRespawnInterval(12 * ENEMY_RESPAWN_RATE)
				->setWeaponName("poisoned tail")
				->setName("Scorpion")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["creature/dog"] = createObject<Creature>("template/creature/dog/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/wood"]), 63 * Dropper::Percent, 12 * ENEMY_MATS_DROP_RATE, 14 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/leather"]), 93 * Dropper::Percent, 12 * ENEMY_MATS_DROP_RATE, 16 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/tinyred"]), 15 * Dropper::Percent)
				->setAttack(25)
				->setDefense(24)
				->setMaxHp(22 * ENEMY_HP_RATE)
				->setCurrentHp(22 * ENEMY_HP_RATE)
				->setRespawnInterval(12 * ENEMY_RESPAWN_RATE)
				->setWeaponName("ferocious bite")
				->setName("Dog")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["creature/bigspider"] = createObject<Creature>("template/creature/bigspider/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/iron"]), 30 * Dropper::Percent, 10 * ENEMY_MATS_DROP_RATE, 24 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/iron"]), 30 * Dropper::Percent, 10 * ENEMY_MATS_DROP_RATE, 24 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/iron"]), 30 * Dropper::Percent, 10 * ENEMY_MATS_DROP_RATE, 24 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/tinyred"]), 18 * Dropper::Percent)
				->setAttack(28)
				->setDefense(30)
				->setMaxHp(34 * ENEMY_HP_RATE)
				->setCurrentHp(34 * ENEMY_HP_RATE)
				->setRespawnInterval(12 * ENEMY_RESPAWN_RATE)
				->setWeaponName("")
				->setName("Big spider")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["creature/wolf"] = createObject<Creature>("template/creature/wolf/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/iron"]), 60 * Dropper::Percent, 11 * ENEMY_MATS_DROP_RATE, 19 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/leather"]), 69 * Dropper::Percent, 7 * ENEMY_MATS_DROP_RATE, 12 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/whitepowder"]), 70  * Dropper::Percent, 7 * ENEMY_MATS_DROP_RATE, 12 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/tinyred"]), 20 * Dropper::Percent)
				->setAttack(31)
				->setDefense(37)
				->setMaxHp(47 * ENEMY_HP_RATE)
				->setCurrentHp(47 * ENEMY_HP_RATE)
				->setRespawnInterval(12 * ENEMY_RESPAWN_RATE)
				->setWeaponName("...")
				->setName("Wolf")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["creature/goblin"] = createObject<Creature>("template/creature/goblin/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/sand"]), 100 * Dropper::Percent, 11 * ENEMY_MATS_DROP_RATE, 15 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/whitepowder"]), 100  * Dropper::Percent, 10 * ENEMY_MATS_DROP_RATE, 23 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/tinyred"]), 23 * Dropper::Percent)
				->drops(shallowCloneTemplate(templates["item/spiderkey"]), ENEMY_BOSS_KEY_RATE * Dropper::Percent)
				->setAttack(37)
				->setDefense(43)
				->setMaxHp(51 * ENEMY_HP_RATE)
				->setCurrentHp(51 * ENEMY_HP_RATE)
				->setRespawnInterval(12 * ENEMY_RESPAWN_RATE)
				->setWeaponName("...")
				->setName("Goblin")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["creature/zombie"] = createObject<Creature>("template/creature/zombie/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/wood"]), 100 * Dropper::Percent, 12 * ENEMY_MATS_DROP_RATE, 18 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/wood"]), 36 * Dropper::Percent, 30 * ENEMY_MATS_DROP_RATE, 58 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/wood"]), 3 * Dropper::Percent, 400 * ENEMY_MATS_DROP_RATE, 600 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/tinyred"]), 26 * Dropper::Percent)
				->setAttack(45)
				->setDefense(41)
				->setMaxHp(82 * ENEMY_HP_RATE)
				->setCurrentHp(82 * ENEMY_HP_RATE)
				->setRespawnInterval(12 * ENEMY_RESPAWN_RATE)
				->setWeaponName("...")
				->setName("Zombie")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["creature/skeleton"] = createObject<Creature>("template/creature/skeleton/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/iron"]), 100 * Dropper::Percent, 7 * ENEMY_MATS_DROP_RATE, 9 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/iron"]), 28 * Dropper::Percent, 26 * ENEMY_MATS_DROP_RATE, 32 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/iron"]), 8 * Dropper::Percent, 90 * ENEMY_MATS_DROP_RATE, 114 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/smallred"]), 11 * Dropper::Percent)
				->setAttack(49)
				->setDefense(83)
				->setMaxHp(49 * ENEMY_HP_RATE)
				->setCurrentHp(49 * ENEMY_HP_RATE)
				->setRespawnInterval(12 * ENEMY_RESPAWN_RATE)
				->setWeaponName("...")
				->setName("Skeleton")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["creature/mummy"] = createObject<Creature>("template/creature/mummy/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/leather"]), 100 * Dropper::Percent, 8 * ENEMY_MATS_DROP_RATE, 12 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/leather"]), 23 * Dropper::Percent, 38 * ENEMY_MATS_DROP_RATE, 49 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/leather"]), 10 * Dropper::Percent, 90 * ENEMY_MATS_DROP_RATE, 100 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/redpowder"]), 14 * Dropper::Percent, 5 * ENEMY_MATS_DROP_RATE, 9 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/smallred"]), 12 * Dropper::Percent)
				->setAttack(54)
				->setDefense(51)
				->setMaxHp(94 * ENEMY_HP_RATE)
				->setCurrentHp(94 * ENEMY_HP_RATE)
				->setRespawnInterval(12 * ENEMY_RESPAWN_RATE)
				->setWeaponName("...")
				->setName("Mummy")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["creature/thief"] = createObject<Creature>("template/creature/thief/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/iron"]), 45 * Dropper::Percent, 30 * ENEMY_MATS_DROP_RATE, 50 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/wood"]), 42 * Dropper::Percent, 52 * ENEMY_MATS_DROP_RATE, 62 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/smallred"]), 13 * Dropper::Percent)
				->setAttack(61)
				->setDefense(80)
				->setMaxHp(65 * ENEMY_HP_RATE)
				->setCurrentHp(65 * ENEMY_HP_RATE)
				->setRespawnInterval(12 * ENEMY_RESPAWN_RATE)
				->setWeaponName("...")
				->setName("Thief")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["creature/orc"] = createObject<Creature>("template/creature/orc/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/iron"]), 45 * Dropper::Percent, 30 * ENEMY_MATS_DROP_RATE, 50 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/wood"]), 42 * Dropper::Percent, 52 * ENEMY_MATS_DROP_RATE, 62 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/redpowder"]), 12 * Dropper::Percent, 7 * ENEMY_MATS_DROP_RATE, 8 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/smallred"]), 15 * Dropper::Percent)
				->setAttack(70)
				->setDefense(94)
				->setMaxHp(67 * ENEMY_HP_RATE)
				->setCurrentHp(67 * ENEMY_HP_RATE)
				->setRespawnInterval(12 * ENEMY_RESPAWN_RATE)
				->setWeaponName("...")
				->setName("Orc")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["creature/grizzlybear"] = createObject<Creature>("template/creature/grizzlybear/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/sand"]), 100 * Dropper::Percent, 40 * ENEMY_MATS_DROP_RATE, 60 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/whitepowder"]), 100  * Dropper::Percent, 20 * ENEMY_MATS_DROP_RATE, 30 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/redpowder"]), 100 * Dropper::Percent, 3 * ENEMY_MATS_DROP_RATE, 4 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/smallred"]), 16 * Dropper::Percent)
				->setAttack(81)
				->setDefense(70)
				->setMaxHp(88 * ENEMY_HP_RATE)
				->setCurrentHp(88 * ENEMY_HP_RATE)
				->setRespawnInterval(12 * ENEMY_RESPAWN_RATE)
				->setWeaponName("...")
				->setName("Grizzly bear")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["creature/gnome"] = createObject<Creature>("template/creature/gnome/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/leather"]), 70 * Dropper::Percent, 30 * ENEMY_MATS_DROP_RATE, 37 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/smallred"]), 18 * Dropper::Percent)
				->drops(shallowCloneTemplate(templates["item/unicornkey"]), ENEMY_BOSS_KEY_RATE * Dropper::Percent)
				->setAttack(86)
				->setDefense(98)
				->setMaxHp(64 * ENEMY_HP_RATE)
				->setCurrentHp(64 * ENEMY_HP_RATE)
				->setRespawnInterval(12 * ENEMY_RESPAWN_RATE)
				->setWeaponName("...")
				->setName("Gnome")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["creature/ghost"] = createObject<Creature>("template/creature/ghost/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/wood"]), 50 * Dropper::Percent, 30 * ENEMY_MATS_DROP_RATE, 50 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/leather"]), 40 * Dropper::Percent, 25 * ENEMY_MATS_DROP_RATE, 31 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/iron"]), 35 * Dropper::Percent, 30 * ENEMY_MATS_DROP_RATE, 33 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/redpowder"]), 70 * Dropper::Percent, 3 * ENEMY_MATS_DROP_RATE, 6 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/smallred"]), 20 * Dropper::Percent)
				->setAttack(99)
				->setDefense(34)
				->setMaxHp(176 * ENEMY_HP_RATE)
				->setCurrentHp(176 * ENEMY_HP_RATE)
				->setRespawnInterval(12 * ENEMY_RESPAWN_RATE)
				->setWeaponName("...")
				->setName("Ghost")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["creature/vampire"] = createObject<Creature>("template/creature/vampire/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/wood"]), 46 * Dropper::Percent, 30 * ENEMY_MATS_DROP_RATE, 50 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/leather"]), 39 * Dropper::Percent, 25 * ENEMY_MATS_DROP_RATE, 32 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/iron"]), 47 * Dropper::Percent, 20 * ENEMY_MATS_DROP_RATE, 26 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/whitepowder"]), 26 * Dropper::Percent, 40 * ENEMY_MATS_DROP_RATE, 60 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/smallred"]), 22 * Dropper::Percent)
				->setAttack(109)
				->setDefense(107)
				->setMaxHp(73 * ENEMY_HP_RATE)
				->setCurrentHp(73 * ENEMY_HP_RATE)
				->setRespawnInterval(12 * ENEMY_RESPAWN_RATE)
				->setWeaponName("...")
				->setName("Vampire")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["creature/giant"] = createObject<Creature>("template/creature/giant/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/wood"]), 36 * Dropper::Percent, 40 * ENEMY_MATS_DROP_RATE, 50 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/leather"]), 29 * Dropper::Percent, 30 * ENEMY_MATS_DROP_RATE, 54 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/iron"]), 31 * Dropper::Percent, 50 * ENEMY_MATS_DROP_RATE, 67 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/smallred"]), 24 * Dropper::Percent)
				->setAttack(121)
				->setDefense(96)
				->setMaxHp(139 * ENEMY_HP_RATE)
				->setCurrentHp(139 * ENEMY_HP_RATE)
				->setRespawnInterval(15 * ENEMY_RESPAWN_RATE)
				->setWeaponName("...")
				->setName("Giant")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["creature/spirit"] = createObject<Creature>("template/creature/spirit/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/sand"]), 60 * Dropper::Percent, 60 * ENEMY_MATS_DROP_RATE, 69 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/whitepowder"]), 47 * Dropper::Percent, 40 * ENEMY_MATS_DROP_RATE, 45 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/redpowder"]), 23 * Dropper::Percent, 20 * ENEMY_MATS_DROP_RATE, 32 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/smallred"]), 26 * Dropper::Percent)
				->setAttack(129)
				->setDefense(55)
				->setMaxHp(191 * ENEMY_HP_RATE)
				->setCurrentHp(191 * ENEMY_HP_RATE)
				->setRespawnInterval(15 * ENEMY_RESPAWN_RATE)
				->setWeaponName("...")
				->setName("Spirit")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["creature/werewolf"] = createObject<Creature>("template/creature/werewolf/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/iron"]), 33 * Dropper::Percent, 20 * ENEMY_MATS_DROP_RATE, 40 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/wood"]), 36 * Dropper::Percent, 40 * ENEMY_MATS_DROP_RATE, 60 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/smallred"]), 28 * Dropper::Percent)
				->drops(shallowCloneTemplate(templates["potion/bigred"]), 10 * Dropper::Percent)
				->drops(shallowCloneTemplate(templates["item/minotaurkey"]), ENEMY_BOSS_KEY_RATE * Dropper::Percent)
				->setAttack(141)
				->setDefense(103)
				->setMaxHp(156 * ENEMY_HP_RATE)
				->setCurrentHp(156 * ENEMY_HP_RATE)
				->setRespawnInterval(15 * ENEMY_RESPAWN_RATE)
				->setWeaponName("...")
				->setName("Werewolf")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["creature/ogre"] = createObject<Creature>("template/creature/ogre/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/iron"]), 43 * Dropper::Percent, 35 * ENEMY_MATS_DROP_RATE, 68 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/wood"]), 31 * Dropper::Percent, 50 * ENEMY_MATS_DROP_RATE, 70 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/dragonskin"]), 26 * Dropper::Percent, 1 * ENEMY_MATS_DROP_RATE, 2 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/red"]), 11 * Dropper::Percent)
				->setAttack(149)
				->setDefense(117)
				->setMaxHp(148 * ENEMY_HP_RATE)
				->setCurrentHp(148 * ENEMY_HP_RATE)
				->setRespawnInterval(15 * ENEMY_RESPAWN_RATE)
				->setWeaponName("...")
				->setName("Ogre")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["creature/medusa"] = createObject<Creature>("template/creature/medusa/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/iron"]), 25 * Dropper::Percent, 40 * ENEMY_MATS_DROP_RATE, 69 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/wood"]), 22 * Dropper::Percent, 100 * ENEMY_MATS_DROP_RATE, 136 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/dragonskin"]), 23 * Dropper::Percent, 4 * ENEMY_MATS_DROP_RATE, 6 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/red"]), 13 * Dropper::Percent)
				->setAttack(161)
				->setDefense(73)
				->setMaxHp(204 * ENEMY_HP_RATE)
				->setCurrentHp(204 * ENEMY_HP_RATE)
				->setRespawnInterval(15 * ENEMY_RESPAWN_RATE)
				->setWeaponName("...")
				->setName("Giant medusa")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["creature/troll"] = createObject<Creature>("template/creature/troll/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/whitepowder"]), 66 * Dropper::Percent, 14 * ENEMY_MATS_DROP_RATE, 21 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/redpowder"]), 55 * Dropper::Percent, 8 * ENEMY_MATS_DROP_RATE, 13 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/dragonskin"]), 26 * Dropper::Percent, 3 * ENEMY_MATS_DROP_RATE, 5 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/red"]), 14 * Dropper::Percent)
				->drops(shallowCloneTemplate(templates["item/hydrakey"]), ENEMY_BOSS_KEY_RATE * Dropper::Percent)
				->setAttack(176)
				->setDefense(85)
				->setMaxHp(315 * ENEMY_HP_RATE)
				->setCurrentHp(315 * ENEMY_HP_RATE)
				->setRespawnInterval(18 * ENEMY_RESPAWN_RATE)
				->setWeaponName("...")
				->setName("Troll")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["creature/cyclops"] = createObject<Creature>("template/creature/cyclops/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/dragonskin"]), 29 * Dropper::Percent, 10 * ENEMY_MATS_DROP_RATE, 14 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/red"]), 16 * Dropper::Percent)
				->setAttack(188)
				->setDefense(233)
				->setMaxHp(136 * ENEMY_HP_RATE)
				->setCurrentHp(136 * ENEMY_HP_RATE)
				->setRespawnInterval(18 * ENEMY_RESPAWN_RATE)
				->setWeaponName("...")
				->setName("Cyclops")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["creature/demon"] = createObject<Creature>("template/creature/demon/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/wood"]), 9 * Dropper::Percent, 90 * ENEMY_MATS_DROP_RATE, 110 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/iron"]), 19 * Dropper::Percent, 90 * ENEMY_MATS_DROP_RATE, 110 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/dragonskin"]), 9 * Dropper::Percent, 9 * ENEMY_MATS_DROP_RATE, 11 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/red"]), 17 * Dropper::Percent)
				->drops(shallowCloneTemplate(templates["item/dragonkey"]), ENEMY_BOSS_KEY_RATE * Dropper::Percent)
				->setAttack(201)
				->setDefense(155)
				->setMaxHp(320 * ENEMY_HP_RATE)
				->setCurrentHp(320 * ENEMY_HP_RATE)
				->setRespawnInterval(18 * ENEMY_RESPAWN_RATE)
				->setWeaponName("...")
				->setName("Demon")
				->setLongName("...")
				->setDescription("...")
				->save();
		// Bosses :
		templates["creature/spiderking"] = createObject<Creature>("template/creature/spiderking/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/leather"]), Dropper::Always, 50 * ENEMY_MATS_DROP_RATE, 60 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/iron"]), Dropper::Always, 40 * ENEMY_MATS_DROP_RATE, 50 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/wood"]), Dropper::Always, 70 * ENEMY_MATS_DROP_RATE, 130 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/sand"]), Dropper::Always, 60 * ENEMY_MATS_DROP_RATE, 75 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/whitepowder"]), Dropper::Always, 60 * ENEMY_MATS_DROP_RATE, 75 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/magicalstone"]), 20 * Dropper::Percent, 1 * ENEMY_MATS_DROP_RATE, 1 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/tinyred"]), 40 * Dropper::Percent, 2, 4)
				->drops(shallowCloneTemplate(templates["potion/smallred"]), 40 * Dropper::Percent, 1, 2)
				->setAttack(56)
				->setDefense(78)
				->setMaxHp(75 * ENEMY_HP_RATE)
				->setCurrentHp(75 * ENEMY_HP_RATE)
				->setRespawnInterval(135 * ENEMY_RESPAWN_RATE)
				->setWeaponName("...")
				->setName("Spider King")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["creature/unicorn"] = createObject<Creature>("template/creature/unicorn/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/leather"]), Dropper::Always, 80 * ENEMY_MATS_DROP_RATE, 97 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/iron"]), Dropper::Always, 60 * ENEMY_MATS_DROP_RATE, 85 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/wood"]), Dropper::Always, 100 * ENEMY_MATS_DROP_RATE, 220 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/magicalstone"]), 50 * Dropper::Percent, 1 * ENEMY_MATS_DROP_RATE, 3 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/tinyred"]), 100 * Dropper::Percent, 5, 15)
				->drops(shallowCloneTemplate(templates["potion/smallred"]), 60 * Dropper::Percent, 3, 8)
				->drops(shallowCloneTemplate(templates["potion/red"]), 40 * Dropper::Percent, 1, 4)
				->drops(shallowCloneTemplate(templates["inventory/bigleatherpack"]), 10 * Dropper::Percent)
				->drops(shallowCloneTemplate(templates["item/pentagonalkey"]), 25 * Dropper::Percent)
				->setAttack(117)
				->setDefense(139)
				->setMaxHp(180 * ENEMY_HP_RATE)
				->setCurrentHp(180 * ENEMY_HP_RATE)
				->setRespawnInterval(180 * ENEMY_RESPAWN_RATE)
				->setWeaponName("...")
				->setName("Unicorn")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["creature/minotaur"] = createObject<Creature>("template/creature/minotaur/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/iron"]), Dropper::Always, 60 * ENEMY_MATS_DROP_RATE, 85 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/wood"]), Dropper::Always, 100 * ENEMY_MATS_DROP_RATE, 220 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/magicalstone"]), 60 * Dropper::Percent, 2 * ENEMY_MATS_DROP_RATE, 4 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/smallred"]), 100 * Dropper::Percent, 7, 11)
				->drops(shallowCloneTemplate(templates["potion/red"]), 45 * Dropper::Percent, 2, 9)
				->drops(shallowCloneTemplate(templates["potion/bigred"]), 30 * Dropper::Percent, 1, 4)
				->drops(shallowCloneTemplate(templates["inventory/hardleatherpack"]), 10 * Dropper::Percent)
				->setAttack(174)
				->setDefense(245)
				->setMaxHp(265 * ENEMY_HP_RATE)
				->setCurrentHp(265 * ENEMY_HP_RATE)
				->setRespawnInterval(225 * ENEMY_RESPAWN_RATE)
				->setWeaponName("...")
				->setName("Minotaur")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["creature/hydra"] = createObject<Creature>("template/creature/hydra/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/dragonskin"]), Dropper::Always, 7 * ENEMY_MATS_DROP_RATE, 12 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/iron"]), Dropper::Always, 60 * ENEMY_MATS_DROP_RATE, 85 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/wood"]), Dropper::Always, 100 * ENEMY_MATS_DROP_RATE, 220 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/magicalstone"]), 90 * Dropper::Percent, 2 * ENEMY_MATS_DROP_RATE, 5 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["potion/red"]), 100 * Dropper::Percent, 5, 7)
				->drops(shallowCloneTemplate(templates["potion/bigred"]), 40 * Dropper::Percent, 4, 7)
				->drops(shallowCloneTemplate(templates["potion/largered"]), 25 * Dropper::Percent, 1, 3)
				->drops(shallowCloneTemplate(templates["inventory/dragonleatherpack"]), 10 * Dropper::Percent)
				->drops(shallowCloneTemplate(templates["item/heptagonalkey"]), 25 * Dropper::Percent)
				->setAttack(219)
				->setDefense(373)
				->setMaxHp(388 * ENEMY_HP_RATE)
				->setCurrentHp(388 * ENEMY_HP_RATE)
				->setRespawnInterval(270 * ENEMY_RESPAWN_RATE)
				->setWeaponName("...")
				->setName("Hydra")
				->setLongName("...")
				->setDescription("...")
				->save();
		templates["creature/dragon"] = createObject<Creature>("template/creature/dragon/1", templateRoom)
				->drops(shallowCloneTemplate(templates["resource/dragonskin"]), Dropper::Always, 7 * ENEMY_MATS_DROP_RATE, 10 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/redpowder"]), Dropper::Always, 10 * ENEMY_MATS_DROP_RATE, 15 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["resource/magicalstone"]), Dropper::Always, 3 * ENEMY_MATS_DROP_RATE, 6 * ENEMY_MATS_DROP_RATE)
				->drops(shallowCloneTemplate(templates["inventory/endragonleatherpack"]), 10 * Dropper::Percent)
				->drops(shallowCloneTemplate(templates["wearable/ensteelsword"]), 10 * Dropper::Percent)
				->setAttack(293)
				->setDefense(511)
				->setMaxHp(554 * ENEMY_HP_RATE)
				->setCurrentHp(554 * ENEMY_HP_RATE)
				->setRespawnInterval(360 * ENEMY_RESPAWN_RATE)
				->setWeaponName("...")
				->setName("Dragon")
				->setLongName("...")
				->setDescription("...")
				->save();
		LOG("WorldCreator") << "Creature templates created. " << LOGF;
		/*
		 *	Spells & scrolls created here
		 */
		templates["spell/telelandcastle"] = createObject<TeleportSpell>("template/teleportspell/landcastle/1")
				->setDestination(rooms["landcastle"])
				->setBaseInt(5)
				->setBaseWis(5)
				->setName("Teleport to Landcastle")
				->setLongName("teleport to landcastle")
				->setDescription("Teleports the caster to Landcastle.")
				->save().unsafeCast<Spell>();
		templates["spellscroll/telelandcastle"] = createObject<SpellScroll>("template/spellscroll/telelandcastle/1", templateRoom)
				->setSpell(templates["spell/telelandcastle"])
				->setName("Teleport to Landcastle scroll")
				->setLongName("a scroll teaching teleport to landcastle")
				->setDescription("It will teach me how to cast a spell... I hope.")
				->save();
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
	}

	void WorldCreator::initRooms() {
		AttackTrap* autoAttack = createObject<AttackTrap>("trap/attack1");
		
		Location* landcastle = createObject<Location>("location/room/landcastle")
				->setRespawnable(true)
				->setName("Landcastle")
				->setDescription("A nice stone castle, having a magical aura surrounding it. You feel the aura and feel much better here. If you are confused what to do, try typing 'help'.")
				->save().unsafeCast<Location>();
		rooms["landcastle"] = landcastle;
		
		createObject<Healing>("trap/healing/landcastle")
				->setRate(300 * Healing::PerHour)
				->setTarget(landcastle)
				->save();
		
		Location* shadowcliff = createObject<Location>("location/room/shadowcliff")
				->setName("Shadowcliff")
				->setDescription("Dark dangerous cliffs. You shouldn't leave the path you are currently on.")
				->save().unsafeCast<Location>();
		rooms["shadowcliff"] = shadowcliff;
		
		Location* appledale = createObject<Location>("location/room/appledale")
				->setName("Appledale")
				->setDescription("A small crafting village.")
				->save().unsafeCast<Location>();
		rooms["appledale"] = appledale;
		
		Location* faygrass = createObject<Location>("location/room/faygrass")
				->setName("Faygrass")
				->setDescription("A vast grass field.")
				->save().unsafeCast<Location>();
		rooms["faygrass"] = faygrass;
		
		Location* winddell = createObject<Location>("location/room/winddell")
				->setName("Winddell")
				->setDescription("A quite windy valley. The birds here seem hungry.")
				->attachTrap(autoAttack, "inside")
				->save().unsafeCast<Location>();
		rooms["winddell"] = winddell;
		
		Location* iceedge = createObject<Location>("location/room/iceedge")
				->setName("Iceedge")
				->setDescription("A narrow icy edge. It looks like you won't be able to climb back.")
				->save().unsafeCast<Location>();
		rooms["iceedge"] = iceedge;
		
		Location* estermoor = createObject<Location>("location/room/estermoor")
				->setName("Estermoor")
				->setDescription("Dark and dangerous moor. The creatures here are kind of big.")
				->save().unsafeCast<Location>();
		rooms["estermoor"] = estermoor;
		
		Location* fayden = createObject<Location>("location/room/fayden")
				->setName("Fayden")
				->setDescription("An astounding cave with some ancient paintings on the walls.")
				->save().unsafeCast<Location>();
		rooms["fayden"] = fayden;
		
		Location* glasscoast = createObject<Location>("location/room/glasscoast")
				->setName("Glasscoat")
				->setDescription("There is a crystal clean sea and a sand which looks like it is shattered glass.")
				->save().unsafeCast<Location>();
		rooms["glasscoast"] = glasscoast;
		
		Location* pinelyn = createObject<Location>("location/room/pinelyn")
				->setName("Pinelyn")
				->setDescription("A dense pine forest. You sense there may be enraged creatures.")
				->attachTrap(autoAttack, "inside")
				->save().unsafeCast<Location>();
		rooms["pinelyn"] = pinelyn;
		
		Location* lightland = createObject<Location>("location/room/lightland")
				->setRespawnable(true)
				->setName("Lightland")
				->setDescription("This looks like a holy clearing. You sense some strange power around which makes you calm and you are starting to feel better.")
				->save().unsafeCast<Location>();
		rooms["lightland"] = lightland;
		
		createObject<Healing>("trap/healing/lightland")
				->setRate(350 * Healing::PerHour)
				->setTarget(lightland)
				->save();
		
		Location* newgate = createObject<Location>("location/room/newgate")
				->setName("Newgate")
				->setDescription("A long passage with a big gate at the end. There seem to be some goblin camps around.")
				->save().unsafeCast<Location>();
		rooms["newgate"] = newgate;
		
		Location* spidernest = createObject<Location>("location/room/spidernest")
				->setName("Spider nest")
				->setDescription("The nest of the infamous spider king. Beware!")
				->save().unsafeCast<Location>();
		rooms["spidernest"] = spidernest;
		
		Location* millburn = createObject<Location>("location/room/millburn")
				->setName("Millburn")
				->setDescription("A field with many old stone mills.")
				->attachTrap(autoAttack, "inside")
				->save().unsafeCast<Location>();
		rooms["millburn"] = millburn;
		
		Location* oldcemetery = createObject<Location>("location/room/oldcemetery")
				->setName("Old cemetary")
				->setDescription("There are many graves around and some of them are opened!")
				->save().unsafeCast<Location>();
		rooms["oldcemetery"] = oldcemetery;
		
		Location* greenwolf = createObject<Location>("location/room/greenwolf")
				->setName("Greenwolf")
				->setDescription("These meadows are known to be ruled by wolfs.")
				->attachTrap(autoAttack, "inside")
				->save().unsafeCast<Location>();
		rooms["greenwolf"] = greenwolf;
		
		Location* magemoor = createObject<Location>("location/room/magemoor")
				->setName("Magemoor")
				->setDescription("These moors are filled with magical energy.")
				->save().unsafeCast<Location>();
		rooms["magemoor"] = magemoor;
		
		Location* greybell = createObject<Location>("location/room/greybell")
				->setName("Greybell")
				->setDescription("A hill with a big old bell at the top.")
				->attachTrap(autoAttack, "inside")
				->save().unsafeCast<Location>();
		rooms["greybell"] = greybell;
		
		Location* littlecave = createObject<Location>("location/room/littlecave")
				->setName("Little cave")
				->setDescription("The cave is deserted. The thiefs nearby may use it as a hideout.")
				->save().unsafeCast<Location>();
		rooms["littlecave"] = littlecave;
		
		Location* rockshore = createObject<Location>("location/room/rockshore")
				->setName("Rockshore")
				->setDescription("Rock cliffs at the edge of the sea.")
				->save().unsafeCast<Location>();
		rooms["rockshore"] = rockshore;
		
		Location* seacave = createObject<Location>("location/room/seacave")
				->setName("Sea cave")
				->setDescription("A small cave found underwater.")
				->save().unsafeCast<Location>();
		rooms["seacave"] = seacave;
		
		Location* brightlake = createObject<Location>("location/room/brightlake")
				->setRespawnable(true)
				->setName("Brightlake")
				->setDescription("A small abandoned village with a healing lake at the town square.")
				->save().unsafeCast<Location>();
		rooms["brightlake"] = brightlake;
		
		createObject<Healing>("trap/healing/lightland")
				->setRate(400 * Healing::PerHour)
				->setTarget(brightlake)
				->save();
		
		Location* ashedge = createObject<Location>("location/room/ashedge")
				->setName("Ashedge")
				->setDescription("A desolated warcamp full of mummies.")
				->save().unsafeCast<Location>();
		rooms["ashedge"] = ashedge;
		
		Location* newoak = createObject<Location>("location/room/newoak")
				->setName("Newoak")
				->setDescription("The stands a majestic oak planted recently.")
				->attachTrap(autoAttack, "inside")
				->save().unsafeCast<Location>();
		rooms["newoak"] = newoak;
		
		Location* crafthill = createObject<Location>("location/room/crafthill")
				->setName("Craftsman's Hill")
				->setDescription("A hill atop Brightlake once used by their finest craftmans.")
				->save().unsafeCast<Location>();
		rooms["crafthill"] = crafthill;
		
		Location* brownpond = createObject<Location>("location/room/brownpond")
				->setName("Brownpond")
				->setDescription("Fairly dirty pond which is full of fish.")
				->save().unsafeCast<Location>();
		rooms["brownpond"] = brownpond;
		
		Location* orangehedge = createObject<Location>("location/room/orangehedge")
				->setName("Orangehedge")
				->setDescription("Hedge lined with many orange trees.")
				->attachTrap(autoAttack, "inside")
				->save().unsafeCast<Location>();
		rooms["orangehedge"] = orangehedge;
		
		Location* stonecircle = createObject<Location>("location/room/stonecircle")
				->setName("Stonecircle")
				->setDescription("")
				->save().unsafeCast<Location>();
		rooms["stonecircle"] = stonecircle;
		
		createObject<Healing>("trap/healing/stonecircle")
				->setRate(500 * Healing::PerHour)
				->setTarget(stonecircle)
				->save();
		
		Location* witchburn = createObject<Location>("location/room/witchburn")
				->setName("Witchburn")
				->setDescription("A once famous home of witches.")
				->save().unsafeCast<Location>();
		rooms["witchburn"] = witchburn;
		
		Location* unicornforest = createObject<Location>("location/room/unicornforest")
				->setName("Unicorn's forest")
				->setDescription("A home of powerful magical unicorn.")
				->save().unsafeCast<Location>();
		rooms["unicornforest"] = unicornforest;
	}

	void WorldCreator::initDoors() {
		createDoor("landcastle-shadowcliff", rooms["landcastle"], rooms["shadowcliff"], true)
				->setGoThroughMessage("You climb over the rocks.")
				->setName("Climb")
				->setLongName("some climbable rocks")
				->save();
		createDoor("appledale-shadowcliff", rooms["appledale"], rooms["shadowcliff"], true)
				->setGoThroughMessage("You follow the pathway.")
				->setName("Pathway")
				->setLongName("a windy stone pathway")
				->save();
		createDoor("landcastle-faygrass", rooms["landcastle"], rooms["faygrass"], true)
				->setGoThroughMessage("You walk over the bridge.")
				->setName("Bridge")
				->setLongName("bridge connecting a meadow with the city")
				->save();
		createDoor("winddell-faygrass", rooms["winddell"], rooms["faygrass"], true)
				->setGoThroughMessage("You walk the stairs.")
				->setName("Stairs")
				->setLongName("large natural stone stairs")
				->save();
		createDoor("winddell-iceedge", rooms["winddell"], rooms["iceedge"], false)
				->setGoThroughMessage("You slide down on the ice.")
				->setName("Icy slide")
				->setLongName("looks like a one way ride")
				->save();
		createDoor("iceedge-landcastle", rooms["iceedge"], rooms["landcastle"], false)
				->setGoThroughMessage("You slide down on the ice and end up at the city gates.")
				->setName("Icy slide")
				->setLongName("looks like another one way ride")
				->save();
		createDoor("faygrass-estermoor", rooms["faygrass"], rooms["estermoor"], true)
				->setGoThroughMessage("You carefully use the walkway.")
				->setName("Muddy walkway")
				->setLongName("there is mud everywhere, but I could try to use it")
				->save();
		createDoor("estermoor-fayden", rooms["estermoor"], rooms["fayden"], false)
				->setGoThroughMessage("You walk into the cave.")
				->setName("Cave opening")
				->setLongName("it leads into some kind of a cave")
				->save();
		createDoor("fayden-estermoor", rooms["fayden"], rooms["estermoor"], false)
				->setGoThroughMessage("You walk out of the cave.")
				->setName("Moor exit")
				->setLongName("you see some moor out there.")
				->save();
		createDoor("glasscoast-fayden", rooms["glasscoast"], rooms["fayden"], false)
				->setGoThroughMessage("You walk into the cave.")
				->setName("Cave opening")
				->setLongName("it leads into some kind of a cave")
				->save();
		createDoor("fayden-glasscoast", rooms["fayden"], rooms["glasscoast"], false)
				->setGoThroughMessage("You walk out of the cave.")
				->setName("Sea exit")
				->setLongName("you see some sea out there.")
				->save();
		createDoor("pinelyn-fayden", rooms["pinelyn"], rooms["fayden"], false)
				->setGoThroughMessage("You walk into the cave.")
				->setName("Cave opening")
				->setLongName("it leads into some kind of a cave")
				->save();
		createDoor("fayden-pinelyn", rooms["fayden"], rooms["pinelyn"], false)
				->setGoThroughMessage("You walk out of the cave.")
				->setName("Forest exit")
				->setLongName("you see some dense forest out there.")
				->save(); 
		createDoor("lightland-pinelyn", rooms["lightland"], rooms["pinelyn"], true)
				->setGoThroughMessage("You cut through the bushes.")
				->setName("Ingrown pathway")
				->setLongName("it is full of bushes, but you may try to cut them down.")
				->save(); 
		createDoor("newgate-pinelyn", rooms["newgate"], rooms["pinelyn"], true)
				->setGoThroughMessage("You carefully walk over the bridge.")
				->setName("Wooden footbridge")
				->setLongName("there are some missing logs, but it is still walkable.")
				->save(); 
		createDoor("newgate-spidernest", rooms["newgate"], rooms["spidernest"], false)
				->setGoThroughMessage("You walk through the gate.")
				->setName("Iron gate")
				->setLongName("a big gate with a symbol of spider requiring some sort of key.")
				->save(); 
		createDoor("spidernest-fayden", rooms["spidernest"], rooms["fayden"], false)
				->setGoThroughMessage("You manage to crawl through the crevice.")
				->setName("Narrow crevice")
				->setLongName("I can barely fit in there, but there is no other option.")
				->save(); 
		createDoor("estermoor-millburn", rooms["estermoor"], rooms["millburn"], true)
				->setGoThroughMessage("You open the gate and walk through it.")
				->setName("Gate")
				->setLongName("a small iron gate.")
				->save(); 
		createDoor("oldcemetery-millburn", rooms["oldcemetery"], rooms["millburn"], true)
				->setGoThroughMessage("You jump over the wall.")
				->setName("Short wall")
				->setLongName("a wall short enough to jump over it.")
				->save(); 
		createDoor("oldcemetery-brightlake", rooms["oldcemetery"], rooms["brightlake"], false)
				->setGoThroughMessage("You ascend the stairs.")
				->setName("Stairs")
				->setLongName("marble stairs leading into a city")
				->save(); 
		createDoor("brightlake-oldcemetery", rooms["brightlake"], rooms["oldcemetery"], false)
				->setGoThroughMessage("You descend the stairs.")
				->setName("Stairs")
				->setLongName("marble stairs leading to a cemetery.")
				->save(); 
		createDoor("rockshore-oldcemetery", rooms["rockshore"], rooms["oldcemetery"], true)
				->setGoThroughMessage("You ford the river.")
				->setName("Ford")
				->setLongName("a place where the river is narrow and shallow.")
				->save(); 
		createDoor("rockshore-seacave", rooms["rockshore"], rooms["seacave"], false)
				->setGoThroughMessage("You swim through the tunnel. The current helps you a lot, but you won't be able to swim back.")
				->setName("Underwater tunnel")
				->setLongName("a tunnel leading underwater.")
				->save(); 
		createDoor("seacave-millburn", rooms["seacave"], rooms["millburn"], false)
				->setGoThroughMessage("You swim through the tunnel.")
				->setName("Underwater tunnel")
				->setLongName("another tunnel with the light at the end.")
				->save(); 
		createDoor("greenwolf-oldcemetery", rooms["greenwolf"], rooms["oldcemetery"], true)
				->setGoThroughMessage("You follow the path.")
				->setName("Wide path")
				->setLongName("a wide stone path.")
				->save(); 
		createDoor("greenwolf-magemoor", rooms["greenwolf"], rooms["magemoor"], true)
				->setGoThroughMessage("You walk through the gate and admire it silently.")
				->setName("Natural gate")
				->setLongName("a huge stone gate.")
				->save(); 
		createDoor("greybell-magemoor", rooms["greybell"], rooms["magemoor"], false)
				->setGoThroughMessage("You walk down the hill.")
				->setName("Windy path")
				->setLongName("a descending path into the moor.")
				->save(); 
		createDoor("magemoor-greybell", rooms["magemoor"], rooms["greybell"], false)
				->setGoThroughMessage("You walk up the hill.")
				->setName("Windy path")
				->setLongName("an ascending path up the hill.")
				->save(); 
		createDoor("littlecave-greybell", rooms["littlecave"], rooms["greybell"], false)
				->setGoThroughMessage("You exit the cave.")
				->setName("Cave exit")
				->setLongName("there is just bright daylight.")
				->save(); 
		createDoor("greybell-littlecave", rooms["greybell"], rooms["littlecave"], false)
				->setGoThroughMessage("You enter the cave.")
				->setName("Cave entrance")
				->setLongName("the cave seems quite small.")
				->save(); 
		createDoor("brightlake-ashedge", rooms["brightlake"], rooms["ashedge"], false)
				->setGoThroughMessage("You board the raft and sail down the river.")
				->setName("Raft")
				->setLongName("you can use it to sail down the river.")
				->save(); 
		createDoor("ashedge-newoak", rooms["ashedge"], rooms["newoak"], false)
				->setGoThroughMessage("You jump down the ledge.")
				->setName("Ledge")
				->setLongName("a not too high ledge, you will be able to jump it down.")
				->save(); 
		createDoor("newoak-crafthill", rooms["newoak"], rooms["crafthill"], false)
				->setGoThroughMessage("You walk up the hill.")
				->setName("Road")
				->setLongName("a road, which was build by some fine stonemakers.")
				->save(); 
		createDoor("crafthill-newoak", rooms["crafthill"], rooms["newoak"], false)
				->setGoThroughMessage("You walk down the hill.")
				->setName("Road")
				->setLongName("a road, which was build by some fine stonemakers.")
				->save(); 
		createDoor("crafthill-brightlake", rooms["crafthill"], rooms["brightlake"], false)
				->setGoThroughMessage("You enjoy the slide.")
				->setName("Stone slide")
				->setLongName("a big slide, you should enjoy a little fun.")
				->save(); 
		createDoor("brownpond-brightlake", rooms["brownpond"], rooms["brightlake"], true)
				->setGoThroughMessage("You walk over the bridge.")
				->setName("Royal bridge")
				->setLongName("a wide bridge with many statues.")
				->save(); 
		createDoor("brownpond-orangehedge", rooms["brownpond"], rooms["orangehedge"], true)
				->setGoThroughMessage("You follow the road.")
				->setName("King's Road")
				->setLongName("a wide old road.")
				->save(); 
		createDoor("stonecircle-orangehedge", rooms["stonecircle"], rooms["orangehedge"], true)
				->setGoThroughMessage("You follow the path.")
				->setName("Beaten path")
				->setLongName("it leads somewhere in the forest")
				->save(); 
		createDoor("witchburn-orangehedge", rooms["witchburn"], rooms["orangehedge"], true)
				->setGoThroughMessage("You carefully open the gate and walk through it.")
				->setName("Ancient gate")
				->setLongName("an ancient city gate")
				->save(); 
		createDoor("witchburn-unicornforest", rooms["witchburn"], rooms["unicornforest"], false)
				->setGoThroughMessage("You walk through the door.")
				->setName("Locked door")
				->setLongName("a door into a tunnel with some kind of lock and an image of unicorn")
				->save(); 
		createDoor("unicornforest-oldcemetery", rooms["unicornforest"], rooms["oldcemetery"], false)
				->setGoThroughMessage("You use the tunnel to leave the forest.")
				->setName("Glowing tunnel")
				->setLongName("a tunnel which emits a faint light somehow")
				->save(); 
	}


	void WorldCreator::initObjects() {
		deepCloneTemplate(templates["creature/smallspider"], rooms["shadowcliff"]);
		deepCloneTemplate(templates["creature/smallspider"], rooms["shadowcliff"]);
		deepCloneTemplate(templates["creature/rat"], rooms["shadowcliff"]);
		
		deepCloneTemplate(templates["crafter/stoneanvil"], rooms["appledale"]);
		deepCloneTemplate(templates["crafter/tanning"], rooms["appledale"]);
		deepCloneTemplate(templates["crafter/alchemy"], rooms["appledale"]);
		
		deepCloneTemplate(templates["creature/smallspider"], rooms["faygrass"]);
		deepCloneTemplate(templates["creature/rat"], rooms["faygrass"]);
		deepCloneTemplate(templates["creature/rat"], rooms["faygrass"]);
		
		deepCloneTemplate(templates["creature/raven"], rooms["winddell"]);
		deepCloneTemplate(templates["creature/raven"], rooms["winddell"]);
		
		ObjectPointer iceedgechest = createObject<Location>("location/chest/iceedge", rooms["iceedge"])
				->setEmptyMessage("There are only some icicles left.")
				->setName("Frozen chest")
				->setDescription("It is covered with ice, but it can contain something useful")
				->attachTrap(createObject<SimpleDamageTrap>("simpledamagetrap/ice/1")
					->setDamage(37)
					->setJustOnce()
					->setDamageMessage("You have cut yourself accidentaly with an icicle. You have received 37 damage.")
					->save(), "examine")
				->save();
		deepCloneTemplate(templates["potion/smallred"], iceedgechest).unsafeCast<Item>()->respawnEvery(210);
		deepCloneTemplate(templates["spellscroll/telelandcastle"], iceedgechest).unsafeCast<Item>()->respawnEvery(600);
		
		deepCloneTemplate(templates["creature/bigrat"], rooms["estermoor"]);
		deepCloneTemplate(templates["creature/bigrat"], rooms["estermoor"]);
		deepCloneTemplate(templates["creature/snake"], rooms["estermoor"]);
		
		deepCloneTemplate(templates["creature/snake"], rooms["fayden"]);
		deepCloneTemplate(templates["creature/snake"], rooms["fayden"]);
		deepCloneTemplate(templates["creature/dog"], rooms["fayden"]);
		
		deepCloneTemplate(templates["creature/scorpion"], rooms["glasscoast"]);
		deepCloneTemplate(templates["creature/scorpion"], rooms["glasscoast"]);
		deepCloneTemplate(templates["creature/scorpion"], rooms["glasscoast"]);
		
		deepCloneTemplate(templates["creature/bigspider"], rooms["pinelyn"]);
		deepCloneTemplate(templates["creature/bigspider"], rooms["pinelyn"]);
		deepCloneTemplate(templates["creature/bigspider"], rooms["pinelyn"]);
		
		deepCloneTemplate(templates["creature/goblin"], rooms["newgate"]);
		deepCloneTemplate(templates["creature/goblin"], rooms["newgate"]);
		deepCloneTemplate(templates["creature/goblin"], rooms["newgate"]);
		
		deepCloneTemplate(templates["creature/spiderking"], rooms["spidernest"]);
		
		deepCloneTemplate(templates["creature/dog"], rooms["millburn"]);
		deepCloneTemplate(templates["creature/dog"], rooms["millburn"]);
		
		deepCloneTemplate(templates["creature/zombie"], rooms["oldcemetery"]);
		deepCloneTemplate(templates["creature/zombie"], rooms["oldcemetery"]);
		deepCloneTemplate(templates["creature/skeleton"], rooms["oldcemetery"]);
		
		deepCloneTemplate(templates["creature/wolf"], rooms["greenwolf"]);
		deepCloneTemplate(templates["creature/wolf"], rooms["greenwolf"]);
		deepCloneTemplate(templates["creature/wolf"], rooms["greenwolf"]);
		
		deepCloneTemplate(templates["creature/zombie"], rooms["magemoor"]);
		deepCloneTemplate(templates["creature/skeleton"], rooms["magemoor"]);
		deepCloneTemplate(templates["creature/mummy"], rooms["magemoor"]);
		
		deepCloneTemplate(templates["creature/thief"], rooms["greybell"]);
		deepCloneTemplate(templates["creature/thief"], rooms["greybell"]);
		
		ObjectPointer littlecavechest = createObject<Location>("location/chest/littlecave", rooms["littlecave"])
				->setEmptyMessage("You find only a small cockroach. Eww.")
				->setName("Ornate chest")
				->setDescription("The are some strange ornaments carved in it.")
				->save();
		deepCloneTemplate(templates["potion/smallred"], littlecavechest).unsafeCast<Item>()->respawnEvery(120);
		deepCloneTemplate(templates["potion/smallred"], littlecavechest).unsafeCast<Item>()->respawnEvery(120);
		deepCloneTemplate(templates["potion/red"], littlecavechest).unsafeCast<Item>()->respawnEvery(210);
		deepCloneTemplate(templates["item/spiderkey"], littlecavechest).unsafeCast<Item>()->respawnEvery(1800);
		
		deepCloneTemplate(templates["creature/orc"], rooms["rockshore"]);
		deepCloneTemplate(templates["creature/orc"], rooms["rockshore"]);
		deepCloneTemplate(templates["creature/orc"], rooms["rockshore"]);
		
		ObjectPointer seacavechest = createObject<Location>("location/chest/seacave", rooms["seacave"])
				->setEmptyMessage("There is only some seaweed left.")
				->setName("Wet small chest")
				->setDescription("The chest is overgrown with some seaweed.")
				->attachTrap(createObject<SimpleDamageTrap>("simpledamagetrap/sea/1")
					->setDamage(73)
					->setJustOnce()
					->setDamageMessage("While opening the chest, you didn't notice a crab hiding behind it. It bit you for 73 damage and ran away.")
					->save(), "examine")
				->save();
		deepCloneTemplate(templates["potion/red"], seacavechest).unsafeCast<Item>()->respawnEvery(300);
		deepCloneTemplate(templates["resource/sand"], seacavechest).unsafeCast<Resource>()->setQuantity(10*ENEMY_MATS_DROP_RATE)->respawnEvery(300);
		deepCloneTemplate(templates["resource/wood"], seacavechest).unsafeCast<Resource>()->setQuantity(8*ENEMY_MATS_DROP_RATE)->respawnEvery(300);
		
		deepCloneTemplate(templates["creature/mummy"], rooms["ashedge"]);
		deepCloneTemplate(templates["creature/mummy"], rooms["ashedge"]);
		
		deepCloneTemplate(templates["creature/grizzlybear"], rooms["newoak"]);
		deepCloneTemplate(templates["creature/grizzlybear"], rooms["newoak"]);
		deepCloneTemplate(templates["creature/grizzlybear"], rooms["newoak"]);
		
		deepCloneTemplate(templates["crafter/stoneanvil"], rooms["crafthill"]);
		deepCloneTemplate(templates["crafter/ironanvil"], rooms["crafthill"]);
		deepCloneTemplate(templates["crafter/tanning"], rooms["crafthill"]);
		
		deepCloneTemplate(templates["creature/gnome"], rooms["brownpond"]);
		deepCloneTemplate(templates["creature/gnome"], rooms["brownpond"]);
		deepCloneTemplate(templates["creature/gnome"], rooms["brownpond"]);
		
		deepCloneTemplate(templates["creature/thief"], rooms["orangehedge"]);
		deepCloneTemplate(templates["creature/thief"], rooms["orangehedge"]);
		
		deepCloneTemplate(templates["crafter/alchemy"], rooms["stonecircle"]);
		
		deepCloneTemplate(templates["creature/ghost"], rooms["witchburn"]);
		deepCloneTemplate(templates["creature/ghost"], rooms["witchburn"]);
		
		deepCloneTemplate(templates["creature/unicorn"], rooms["unicornforest"]);
		
		createObject<Interactive>("board/landcastle", rooms["landcastle"])
			->reads("Gotcha! Nothing here.")
			->setLongName("Board with little roof with some torn paper attached.")
			->setName("WANTED! board")
			->save();
		
		createObject<Interactive>("speaker/landcastle", rooms["landcastle"])
			->says("Everybody please help!\nSome fuckin' rat 100%% stole my Ring. My wife's gonna be so mad! Bring it to me, %u, and gold won't miss you.")
			->setLongName("")
			->setName("Sauron")
			->save();
	}



	void WorldCreator::bigBang() {
		LOGH("Big bang");
		LOG("WorldCreator") << "Big bang initiated. Everything is being created... " << LOGF;
		createObject<MTATrap>("trap/mta");
		
		LOGH("Rooms");
		initRooms();
		LOG("WorldCreator") << "Rooms created. " << LOGF;
		LOGH("Templates");
		initTemplates();
		LOG("WorldCreator") << "Templates created. " << LOGF;
		LOGH("Doors");
		initDoors();
		LOG("WorldCreator") << "Doors created. " << LOGF;
		LOGH("Admins");
		initAdmins();
		LOG("WorldCreator") << "Admins created. " << LOGF;
		LOGH("World");
		initObjects();
		LOG("WorldCreator") << "World created. " << LOGF;
		LOGH("Testing stuff");

		AttackTrap* autoAttack = createObject<AttackTrap>("trap/attack");
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

				createObject<Potion>("potion/redhealing/" + RANDID, potionChest)
						->setType(Potion::PotionType::Healing)
						->setStrength(200)
						->setName("Red elixir")
						->setLongName("a red elixir to test uncertainity")
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
						->setMainStat(Human::Crafting)
						->addStatReq(createObject<StatReq>("statreq/craft/" + RANDID)
							->setStat(Human::Crafting)->setValue(6)->save())
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
						->setAttackBonus(9)
						->setSlot(Wearable::Weapon)
						->addStatReq(statReq(Human::Strength, 11))
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
						->save(), "examine")
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

