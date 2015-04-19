#include "IDescriptable.hpp"
#include "../../Game/ActionDescriptor.hpp"
#include "../../Actions/ExamineAction.hpp"


namespace Dungeon {

	string IDescriptable::getDescription() const {
		if (description == "")
			return "Nothing much really. Just " + getLongName() + ".";
		return description;
	}

	IDescriptable* IDescriptable::setDescription(const string& description) {
		this->description = description;
		return this;
	}

	string IDescriptable::getLongName() const {
		if (longName == "")
			return name;
		return longName;
	}

	IDescriptable* IDescriptable::setLongName(const string& longName) {
		this->longName = longName;
		return this;
	}

	string IDescriptable::getName() const {
		return name;
	}

	IDescriptable* IDescriptable::setName(const string& name) {
		this->name = name;
		return this;
	}

	void IDescriptable::registerProperties(IPropertyStorage& storage) {
		storage.have(name, "descriptable-name", "Short name")
				.have(longName, "descriptable-longName", "Long name")
				.have(description, "descriptable-description", "Full description");
		Base::registerProperties(storage);
	}

	void IDescriptable::getActions(ActionDescriptor* ad) {
		ad->getActionList().addAction(new ExamineEction())
				->addTarget(this);
	}

	void IDescriptable::examine(ActionDescriptor* ad) {
		triggerTraps("examine", ad);
		if (ad) {
			*ad << getDescription() << eos;
		}
	}

	string IDescriptable::getDescriptionSentence() {
		return "There is " + this->getName() + ".";
	}

	string IDescriptable::getGroupDescriptionSentence(vector<ObjectPointer> others) {
		if (others.size() == 0) return "";
		else if (others.size() == 1) return getDescriptionSentence();

		string sentence = "There is ";
		for (size_t i = 0; i < others.size() - 1; i++) {
			if (i != 0) sentence += ", ";
			sentence += others.at(i).safeCast<IDescriptable>()->getName();
		}

		sentence += " and " + others.at(others.size() - 1).safeCast<IDescriptable>()->getName() + ".";
		return sentence;
	}

	NONPERSISTENT_IMPLEMENTATION(IDescriptable, IDescriptable)


}

