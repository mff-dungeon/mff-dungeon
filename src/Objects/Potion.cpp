#include "Potion.hpp"
#include "../Utils/RegexMatcher.hpp"
#include "../Actions/CallbackAction.hpp"
#include "../Game/ActionDescriptor.hpp"
#include "../Game/GameManager.hpp"

namespace Dungeon {

	Potion* Potion::setType(PotionType type) {
		this->type = type;
		return this;
	}

	Potion* Potion::setStrength(int strength) {
		this->strength = strength;
		return this;
	}

	void Potion::getActions(ActionList* list, ObjectPointer callee) {
		list->addAction(new DrinkPotionAction)
				->addTarget(this)
				->useActionFor(this, list);
	}

	string Potion::getDescription() const {
		stringstream ss;
		ss << Item::getDescription();
		switch (getType()) {
			case Potion::PotionType::Healing:
				ss << "When drunk, it can heal you " << getStrength() << " hitpoints.";
				break;
			case Potion::PotionType::NoEffect:
			default:
				ss << "It is uncertain what it will do when drunk.";
		}
		return ss.str();
	}

	void Potion::registerProperties(IPropertyStorage& storage) {
		storage.have((int&) type, "potion-type", "Potion type (0 - ineffective, 1 - healing, 2 - poison)")
				.have(strength, "potion-strength", "Potion strength");
		Item::registerProperties(storage);
	}

	void DrinkPotionAction::explain(ActionDescriptor* ad) {
		*ad << "Use 'drink ...' to drink a potion you have or see.\n" << eos;
	}

	bool DrinkPotionAction::match(const string& command, ActionDescriptor* ad) {
		if (RegexMatcher::match("drink .+", command)) {
			selectBestTarget(command.substr(6), ad);
			return true;
		}
		return false;
	}

	void DrinkPotionAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		target.assertType<Potion>("You're too drunk. Hic.");
		Potion* potion = target.unsafeCast<Potion>();
		*ad << "You've drunk " + potion->getName() + "." << eos;
		int lastHp = ad->getCaller()->getCurrentHp();
		switch (potion->getType()) {
			case Potion::PotionType::Healing:
				ad->getCaller()->changeHp(potion->getStrength());
				break;
			case Potion::PotionType::Poison:
				ad->getCaller()->changeHp(-potion->getStrength());
				break;
			case Potion::PotionType::NoEffect:
			default:
				*ad << "... and it did nothing." << eos;
		}
		int curHp = ad->getCaller()->getCurrentHp();
		if (curHp > lastHp)
			*ad << "You've healed " << curHp - lastHp << " hitpoints." << eos;
		else if (lastHp > curHp)
			*ad << "You've lost " << lastHp - curHp << " hitpoints." << eos;
		ad->getGM()->deleteObject(potion);
	}

	PERSISTENT_IMPLEMENTATION(Potion)
}