#include "Stats.hpp"
#include "../../Utils/RegexMatcher.hpp"
#include "../../Utils/FuzzyStringMatcher.hpp"
#include "../../Game/ActionDescriptor.hpp"

namespace Dungeon {

	void RaiseStatAction::explain(ActionDescriptor* ad) {
		*ad << "raise ... - raise your character stat." << eos;
	}

	bool RaiseStatAction::match(string command, ActionDescriptor* ad) {
		if (RegexMatcher::match("raise .+", command)) {
			selectStat(command.substr(6), ad);
			return true;
		}
		return false;
	}

	void RaiseStatAction::selectStat(string statName, ActionDescriptor* ad) {
		FuzzyStringMatcher<Human::Stats> matcher;
		matcher.add("strength", Human::Strength)
				.add("str", Human::Strength)
				.add("dexterity", Human::Dexterity)
				.add("dex", Human::Dexterity)
				.add("vitality", Human::Vitality)
				.add("vit", Human::Vitality)
				.add("intelligence", Human::Intelligence)
				.add("int", Human::Intelligence)
				.add("wisdom", Human::Wisdom)
				.add("wis", Human::Wisdom)
				.add("crafting", Human::Crafting)
				.add("craft", Human::Crafting)
				.add("alchemy", Human::Alchemy)
				.add("alch", Human::Alchemy);
		try {
			this->selectedStat = matcher.find(statName);
		}		catch (exception e) {

		}
	}

	void RaiseStatAction::commit(ActionDescriptor* ad) {
		if (selectedStat == Human::End) {
			*ad << "There was no such stat found." << eos;
			return;
		}
		Human* h = ad->getCaller();
		if (h->getFreePoints() == 0) {
			*ad << "You have no free points to distribute." << eos;
			return;
		}
		h->useStatPoint(selectedStat);
	}

	Human::Stats StatReq::getStat() const {
		return stat;
	}

	StatReq* StatReq::setStat(Human::Stats stat) {
		this->stat = stat;
		return this;
	}

	int StatReq::getValue() const {
		return value;
	}

	StatReq* StatReq::setValue(int value) {
		this->value = value;
		return this;
	}

	void StatReq::registerProperties(IPropertyStorage& storage) {
		storage.have((int&) stat, "statreq-type", "The required stat.")
				.have(value, "statreq-value", "The required stat value.");
	}

	PERSISTENT_IMPLEMENTATION(StatReq)
}
