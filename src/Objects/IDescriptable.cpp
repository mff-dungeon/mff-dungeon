#include "IDescriptable.hpp"

namespace Dungeon {
	
	string IDescriptable::getDescription() const {
		if (description == "")
			return "Nothing much really. Just " + getLongName() + ".";
		return description;
	}

	IDescriptable* IDescriptable::setDescription(string description) {
		this->description = description;
		return this;
	}

	string IDescriptable::getLongName() const {
		if (longName == "")
			return name;
		return longName;
	}

	IDescriptable* IDescriptable::setLongName(string longName) {
		this->longName = longName;
		return this;
	}

	string IDescriptable::getName() const {
		return name;
	}

	IDescriptable* IDescriptable::setName(string name) {
		this->name = name;
		return this;
	}
	
	void IDescriptable::registerProperties(IPropertyStorage& storage) {
		storage.have(name, "Short name")
			.have(longName, "Long name")
			.have(description, "Full description");
		IObject::registerProperties(storage);
	}

    string IDescriptable::getDescriptionSentence() {
        return "There is " + this->getName() + ".";
    }
    
    string IDescriptable::getGroupDescriptionSentence(vector<IDescriptable *> others) {
        if (others.size() == 0) return "";
        else if (others.size() == 1) return getDescriptionSentence();
        
        string sentence = "There are ";
        for (int i = 0; i < others.size() - 1; i++) {
            if (i != 0) sentence += ", ";
            sentence += others.at(i)->getName();
        }
        
        sentence += " and " + others.at(others.size() - 1)->getName() + ".";
        return sentence;
    }

}

