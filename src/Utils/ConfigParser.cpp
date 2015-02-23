#include <fstream>
#include <ctype.h>
#include "ConfigParser.hpp"

namespace Dungeon {
	ConfigParser::ConfigParser(const string& fileName) {
		ifstream input(fileName);
		if(!input.good()) {
			throw InvalidFileException("Couldn't find or open config file - " + fileName + ".");
		}
		string line, key, val;
		int lc = 0;
		while(input.good()) {
			getline(input, line);
			lc++;
			if(line.find("#") == 0 || isEmpty(line)) continue;	// Comment/blank lines are skipped
			if(line.find("=") == string::npos) {
				throw InvalidFileException("Invalid config file format on line " + to_string(lc) + ".");
			}
			key = line.substr(0, line.find("="));
			val = line.substr(line.find("=")+1, line.length());
			Utils::trim(key);
			Utils::trim(val);
			loadedValues.insert(pair<string, string>(key, val));
		}
		input.close();
	}
	
	bool ConfigParser::isEmpty(const string& s) const {
		for(char c : s) {
			if(!isspace(c)) return false;
		}
		return true;
	}
	
	string ConfigParser::getString(const string& key) const {
		map<string, string>::const_iterator it = loadedValues.find(key);
		if(it == loadedValues.end()) {
			throw InvalidFieldException("No field with identifier " + key + " found.", key);
		}
		return (*it).second;
	}
	
	string ConfigParser::getString(const string& key, const function<bool(string)>& predicate, string error) const {
		map<string, string>::const_iterator it = loadedValues.find(key);
		if(it == loadedValues.end()) {
			throw InvalidFieldException("No field with identifier " + key + " found.", key);
		}
		if(!predicate((*it).second)) {
			throw InvalidFieldException(error, key);
		}
		return (*it).second;
	}

	
	bool ConfigParser::getBool(const string& key) const {
		map<string, string>::const_iterator it = loadedValues.find(key);
		if(it == loadedValues.end()) {
			throw InvalidFieldException("No field with identifier " + key + " found.", key);
		}
		string val = Utils::decapitalize(string((*it).second));
		if(val == "true" || val == "t") return true;
		else if(val == "false" || val == "f") return false;
		else throw InvalidFieldException("Invalid field value, expected boolean value", key);
	}

	double ConfigParser::getDouble(const string& key) const {
		map<string, string>::const_iterator it = loadedValues.find(key);
		if(it == loadedValues.end()) {
			throw InvalidFieldException("No field with identifier " + key + " found.", key);
		}
		double d;
		try {
			d = stod((*it).second);
		}
		catch(exception& e) {
			throw InvalidFieldException("Invalid field value, expected valid double value", key);
		}
		return d;
	}

	double ConfigParser::getDouble(const string& key, const function<bool(double)> &predicate, string error) const {
		double d = getDouble(key);
		if(!predicate(d)) {
			throw InvalidFieldException(error, key);
		}
		return d;
	}

	int ConfigParser::getInt(const string& key) const {
		map<string, string>::const_iterator it = loadedValues.find(key);
		if(it == loadedValues.end()) {
			throw InvalidFieldException("No field with identifier " + key + " found.", key);
		}
		int i;
		try {
			i = stoi((*it).second);
		}
		catch(exception& e) {
			throw InvalidFieldException("Invalid field value, expected valid integer value", key);
		}
		return i;
	}

	int ConfigParser::getInt(const string& key, const function<bool(int)> &predicate, string error) const {
		int i = getInt(key);
		if(!predicate(i)) {
			throw InvalidFieldException(error, key);
		}
		return i;
	}

}