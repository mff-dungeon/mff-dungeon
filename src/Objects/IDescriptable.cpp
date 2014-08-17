#include "IDescriptable.hpp"
#include <sstream>

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
	
	bool IDescriptable::isDescriptable() const {
		return getName() != "";
	}
	
	void IDescriptable::serialize(Archiver& stream) {
		if (stream.isStoring()) {
			stream << name;
			stream << longName;
			stream << description;
		} else {
			stream >> name;
			stream >> longName;
			stream >> description;
		}
		IObject::serialize(stream);
	}
    
    string IDescriptable::getDescriptionSentence() {
        stringstream ss;
        ss << "There is " << this->getName() << ".";
        return ss.str();
    }

}

