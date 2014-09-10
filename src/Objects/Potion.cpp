#include "Potion.hpp"
#include "../Actions/CallbackAction.hpp"
#include "../ActionDescriptor.hpp"

namespace Dungeon {
	
	Potion::Potion() {

	}
	
	Potion::Potion(objId id) : Item(id) {
		
	}

	Potion::~Potion() {

	}
	
	Potion* Potion::setType(PotionType type) {
		this->type = type;
		return this;
	}
	
	Potion* Potion::setStrength(int strength) {
		this->strength = strength;
		return this;
	}
	
	void Potion::getActions(ActionList* list, ObjectPointer callee) {
		Item::getActions(list, callee);
		DrinkPotionAction* action = new DrinkPotionAction;
		action->addTarget(this);
		list->addAction(action);
	}
	
	string Potion::getDescription() const {
		stringstream ss;
		ss << Item::getDescription();
		switch(getType()) {
			case Potion::PotionType::Healing:
				ss << "When drunk, it can heal you " << getStrength() << " hitpoints. ";
				break;
			case Potion::PotionType::NoEffect:
			default:
				ss << "It is uncertain what it will do when drunk. ";
		}
		return ss.str();
	}

	void Potion::registerProperties(IPropertyStorage& storage) {
		storage.have((int&) type, "potion-type", "Potion type (0 - ineffective, 1 - healing, 2 - poison)")
			.have(strength, "potion-strength", "Potion strength");
		Item::registerProperties(storage);
	}
	
	void DrinkPotionAction::explain(ActionDescriptor* ad) {
		*ad << "Use 'drink ...' to drink a potion you have or see.\n";
	}
	
	bool DrinkPotionAction::matchCommand(string command) {
		return RegexMatcher::match("drink .+", command);
	}
        
	void DrinkPotionAction::commit(ActionDescriptor* ad) {	
		commitOnBestTarget(ad, ad->in_msg.substr(6));
	}
        
	void DrinkPotionAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		target.assertType<Potion>("You're too drunk. Hic.");
		Potion* potion = target.unsafeCast<Potion>();
		*ad << "You've drunk " + potion->getName() + ". ";
		int lastHp = ad->getAlive()->getCurrentHp();
		switch(potion->getType()) {
			case Potion::PotionType::Healing:
				ad->getAlive()->changeHp(potion->getStrength());
				break; 
			case Potion::PotionType::Poison:
				ad->getAlive()->changeHp(-potion->getStrength());
				break; 
			case Potion::PotionType::NoEffect:
			default:
				*ad << "... and it did nothing.";
		}
		int curHp =  ad->getAlive()->getCurrentHp();
		if (curHp > lastHp) 
			*ad << "You've healed " <<  curHp - lastHp << " hitpoints. ";
		else if (lastHp > curHp)
			*ad << "You've lost " <<  lastHp - curHp << " hitpoints. ";
		ad->getGM()->deleteObject(potion);
	}

	PERSISTENT_IMPLEMENTATION(Potion)
}