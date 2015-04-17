#include "Archiver.hpp"
#include <vector>
#include <stdexcept>

using namespace std;

namespace Dungeon {
	
	Archiver::Archiver(stringstream& stream, bool isStoring) : storing(isStoring), stream(stream) { }

	Archiver::~Archiver() {
	}

	bool Archiver::isStoring() const {
		return storing;
	}

	void Archiver::setDirection(bool isStoring) {
		this->storing = isStoring;
	}

	string Archiver::printStream() {
		return this->stream.str();
	}

	Archiver& Archiver::operator<<(const string& s) {
		size_t l = s.length();
		write(&l, sizeof (l));
		write(s.c_str(), sizeof (char) * l);
		return *this;
	}

	Archiver& Archiver::operator>>(string& s) {
		size_t l = 0;
		read(&l, sizeof (l));
		char mem [l];
		read(mem, sizeof (char) * l);
		s = string(mem, l);
		return *this;
	}

	Archiver& Archiver::operator<<(int val) {
		write(&val, sizeof (int));
		return *this;
	}

	Archiver& Archiver::operator>>(int& val) {
		read(&val, sizeof (int));
		return *this;
	}

	Archiver& Archiver::operator<<(long val) {
		write(&val, sizeof (long));
		return *this;
	}

	Archiver& Archiver::operator>>(long& val) {
		read(&val, sizeof (long));
		return *this;
	}

	Archiver& Archiver::operator<<(bool b) {
		write(&b, sizeof (bool));
		return *this;
	}

	Archiver& Archiver::operator>>(bool& b) {
		read(&b, sizeof (bool));
		return *this;
	}

	IPropertyStorage& Archiver::have(int& prop, const string& id, const string& desc, bool editable) {
		if (storing)
			* this << prop;
		else *this >> prop;
		return *this;
	}

	IPropertyStorage& Archiver::have(long& prop, const string& id, const string& desc, bool editable) {
		if (storing)
			* this << prop;
		else *this >> prop;
		return *this;
	}

	IPropertyStorage& Archiver::have(string& prop, const string& id, const string& desc, bool editable) {
		if (storing)
			* this << prop;
		else *this >> prop;
		return *this;
	}

	IPropertyStorage& Archiver::have(bool& prop, const string& id, const string& desc, bool editable) {
		if (storing)
			* this << prop;
		else *this >> prop;
		return *this;
	}

	void Archiver::write(const void* buffer, size_t length) {
		stream.write((const char*) buffer, length);
		if (stream.bad())
			throw runtime_error("Archiver::write Error");
	}

	void Archiver::read(void* buffer, size_t length) {
		stream.read((char*) buffer, length);
		if (stream.bad())
			throw runtime_error("Archiver::read Error");
	}
}

