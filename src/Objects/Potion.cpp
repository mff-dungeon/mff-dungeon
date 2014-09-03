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
	
	void Potion::getActions(ActionList* list, IObject* callee) {
		Item::getActions(list, callee);
		DrinkPotionAction* action = new DrinkPotionAction;
		action->addTarget(this->getObjectPointer());
		list->addAction(action);
	}
	
	void Potion::registerProperties(IPropertyStorage& storage) {
		storage.have((int&) type, "potion-type", "Potion type (0 - ineffective, 1 - healing)")
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
		if (!target->instanceOf(Potion))
			return;
		Potion* potion = (Potion*) target.get();
		*ad << "You've drunk " + potion->getName() + ". ";
		switch(potion->getType()) {
			case Potion::PotionType::Healing: {
				int lastHp = ad->getAlive()->getCurrentHp();
				ad->getAlive()->changeHp(potion->getStrength());
				*ad << "You've healed " << lastHp - ad->getAlive()->getCurrentHp() << " hitpoints";
				break; 
			}
			case Potion::PotionType::NoEffect:
			default:
				*ad << "... and it did nothing.";
		}
		ad->getGM()->deleteObject(potion);
	}

	PERSISTENT_IMPLEMENTATION(Potion)
}