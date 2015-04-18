#include "RandomString.hpp"

namespace Dungeon {

	RandomString* RandomString::instance = new RandomString();

	RandomString::RandomString() {
		random.seed(hw_random());
	}

	RandomString::EndOfMessage* endr () {
        throw "Method endr is not callable.";
	}

	RandomString& RandomString::get() {
		return *RandomString::instance;
	}

	string RandomString::getRandom() {
		if (messages.size() == 0) return "";
		uniform_int_distribution<> dist(0, messages.size() - 1);
		return messages[dist(random)];
	}

	RandomString& RandomString::operator<<(RandomString::EndOfMessage* (*endofmessage)()) {
		messages.push_back(current.str());
		current.str("");
		return *this;
	}
}

