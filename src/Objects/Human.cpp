#include "Human.hpp"

namespace Dungeon {
	Human::Human() {
	}

	Human::Human(const Human& orig) {
	}

	Human::~Human() {
	}
	
	string Human::getLongName() const {
		return "user " + getUsername() + ", " + getContact();
	}

	string Human::getName() const {
		return getUsername();
	}

    Human* Human::setContact(string contact) {
    	this->contact = contact;
		return this;
    }

    string Human::getContact() const {
    	return contact;
    }

    Human* Human::setUsername(string username) {
    	this->username = username;
		return this;
    }

    string Human::getUsername() const {
    	return username;
    }
	
	void Human::serialize(Archiver& stream) {
		if (stream.isStoring()) {
			stream << username;
			stream << contact;
		} else {
			stream >> username;
			stream >> contact;
		}
		Alive::serialize(stream);
	}


	PERSISTENT_IMPLEMENTATION(Human)
}