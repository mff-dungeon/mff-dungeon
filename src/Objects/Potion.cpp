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
	
	void Potion::serialize(Archiver& stream) {
		if (stream.isStoring()) {
			stream << type;
			stream << strength;
		} else {
			int t;
			// To deal with enum
			stream >> t;
			stream >> strength;
			this->setType((PotionType) t);
		}
		Item::serialize(stream);
	}
	
	void DrinkPotionAction::explain(ActionDescriptor* ad) {
		*ad << "Use 'drink ...' to drink a potion you have or see.\n";
	}
	
	void DrinkPotionAction::commit(ActionDescriptor* ad) {
		ObjectPointer* current = ad->getAlive()->getLocation();
		for (auto& pair : targets) {
			if (pair.second->getId() == current->getId()) continue;
			commitOnTarget(ad, pair.second); // TODO Implement object matching...
			return;
		}
	}
	
	bool DrinkPotionAction::matchCommand(string command) {
		return RegexMatcher::match("drink .+", command);
	}
	
	void DrinkPotionAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer* target) {	
		Potion* potion = (Potion*) target->get();
		*ad << "You've drunk " + potion->getName() + ". ";
		switch(potion->getType()) {
			case Potion::PotionType::Healing:
				ad->getAlive()->hitpoints += potion->getStrength();
				*ad << "You've healed " + to_string(potion->getStrength()) + " hitpoints";
				break;
			case Potion::PotionType::NoEffect:
			default:
				*ad << "... and it did nothing.";
		}
		ad->getGM()->deleteObject(potion);
	}

	PERSISTENT_IMPLEMENTATION(Potion)
}