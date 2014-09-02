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
		storage.have((int&) type, "Potion type (0 - ineffective, 1 - healing)")
			.have(strength, "Potion strength");
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
		Potion* potion = (Potion*) target.get();
		*ad << "You've drunk " + potion->getName() + ". ";
		switch(potion->getType()) {
			case Potion::PotionType::Healing: {
				int healed = potion->getStrength();	// Need to do this to write correct reply
				if(healed > (ad->getAlive()->getMaxHp() - ad->getAlive()->getCurrentHp())) {
					healed = (ad->getAlive()->getMaxHp() - ad->getAlive()->getCurrentHp());
				}
				ad->getAlive()->changeHp(healed);
				*ad << "You've healed " + to_string(healed) + " hitpoints";
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