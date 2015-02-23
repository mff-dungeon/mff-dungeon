#ifndef CONFIGPARSER_HPP
#define	CONFIGPARSER_HPP

#include "../common.hpp"
#include <string>
#include <fstream>
#include <map>

namespace Dungeon {
	/*
	 * Used for parsing Config files.
	 * It reads the entire file on this object's creation and stores it into a map.
	 * Then, using get methods, the values are parsed and returned.
	 * Currently supports string, double, int and bool values.
	 */
	class ConfigParser {
	public:
		ConfigParser(const string& fileName);
		~ConfigParser() {};
		
		string getString(const string& key) const;
		string getString(const string& key, const function<bool(string)> &predicate, string error) const;
		int getInt(const string& key) const;
		int getInt(const string& key, const function<bool(int)> &predicate, string error) const;
		double getDouble(const string& key) const;
		double getDouble(const string& key, const function<bool(double)> &predicate, string error) const;
		bool getBool(const string& key) const;
		
		/*
		 * Used to notify that there was a field which couldn't be found or understood
		 */
		class InvalidFieldException : public exception {
		public:
			InvalidFieldException(const string& msg, const string& fieldName) {
				this->msg = msg;
				this->fieldName = fieldName;
			}
			virtual const char* what() const _GLIBCXX_USE_NOEXCEPT {
				return msg.c_str();
			};
			string msg;
			string fieldName;
		};
		
		/*
		 * Used when the parsed file is not found
		 */
		class InvalidFileException : public exception {
		public:
			InvalidFileException(const string& msg) {
				this->msg = msg;
			}
			virtual const char* what() const _GLIBCXX_USE_NOEXCEPT {
				return msg.c_str();
			};
			string msg;
		};
		
	private:
		ConfigParser(ConfigParser const&) = delete;
		void operator=(ConfigParser const&) = delete;
		
		bool isEmpty(const string& s) const;
		
		map<string, string> loadedValues;
	};
}

#endif	/* CONFIGPARSER_HPP */

