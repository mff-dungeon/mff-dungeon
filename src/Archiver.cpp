#include "Archiver.hpp"
#include <vector>

namespace Dungeon {

	Archiver::Archiver(stringstream* stream, bool isStoring) : stream(stream), storing(isStoring) {

	}

	Archiver::~Archiver() {
		// clear stream
	}

	bool Archiver::isStoring() const { 
		return storing; 
	}

	void Archiver::setDirection(bool isStoring) { 
		this->storing = isStoring; 
	}

	string Archiver::printStream() {
		return this->stream->str();
	}

	Archiver& Archiver::operator<<(const string& s) {
		int l = s.length();
		*this << l;
		write(s.c_str(), sizeof (char) * l);
		return *this;
	}

	Archiver& Archiver::operator>>(string& s) {
		int l = -1;
		*this >> l;
		vector<char> mem(l + 1);
		char* pChars = &mem[0];
		read(pChars, sizeof(char) * l);
		mem[l] = 0;
		s = pChars;
		return *this;
	}

	Archiver& Archiver::operator<<(int val) {
		write(&val, sizeof(int));
		return *this;
	}

	Archiver& Archiver::operator>>(int& val) {
		read(&val, sizeof(int));
		return *this;
	}

	Archiver& Archiver::operator<<(bool b) {
		write(&b, sizeof(bool));
		return *this;
	}

	Archiver& Archiver::operator>>(bool& b) {
		read(&b, sizeof(bool));
		return *this;
	}

	void Archiver::write(const void* buffer, size_t length) {
		stream->write((const char*) buffer, length);
		if(! *stream)
			throw "Archiver::write Error";
	}

	void Archiver::read(void* buffer, size_t length) {
		stream->read((char*) buffer, length);
		if(! *stream) 
			throw "Archiver::read Error";
	}
}

