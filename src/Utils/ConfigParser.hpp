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

		/**
		 * Returns string value associated with the supplied key.
         * @param key key to be searched for
		 * @throws InvalidFieldException when no such field in configuration file is found
         * @return a string value found in config
         */
		string getString(const string& key) const;

		/**
		 * Returns string value associated with the supplied key. The value is also tested
		 * using the predicate function, which allows to filter results (create enums, etc.)
         * @param key key to be searched for
         * @param predicate a function taking string and returning bool, which verifies the loaded value
         * @param error a message handed to the exception, if the verification fails
		 * @throws InvalidFieldException when no such field in configuration file is found or the value didn't pass the test
         * @return a string value found in config
         */
		string getString(const string& key, const function<bool(string)> &predicate, string error) const;

		/**
		 * Returns an integer value associated with the supplied key.
         * @param key key to be searched for
		 * @throws InvalidFieldException when there is no such field in the configuration file or it is not a valid integer
         * @return an integer value found in config
         */
		int getInt(const string& key) const;

		/**
		 * Returns an integer value associated  with the supplied key. The value is also tested using
		 * the supplied predicate function to filter the results.
         * @param key key to be searched for
         * @param predicate a function taking an integer and returning bool, which verifies the loaded value
         * @param error a message handed to the exception, if the verification fails
		 * @throws InvalidFieldException when there is no such field in the configuration file,
		 *	it is not a valid integer value or it didn't pass the value test
         * @return an integer value found in config
         */
		int getInt(const string& key, const function<bool(int)> &predicate, string error) const;

		/**
		 * Returns a double value associated with the supplied key.
         * @param key key to be searched for
		 * @throws InvalidFieldException when there is no such field in the configuration file or
		 *	it is not a valid double value
         * @return a double value found in config
         */
		double getDouble(const string& key) const;

		/**
		 * Returns a double value associated with the supplied key. The value is also tested using
		 * the supplied predicate function to filter the results.
         * @param key key to be searched for
         * @param predicate a function taking a double and returning bool, which verifies the loaded value
         * @param error a message handed to the exception, if the verification fails
		 * @throws InvalidFieldException when there is no such field in the configuration file,
		 *	it is not a valid double value or it didn't pass the value test
         * @return a double value found in config
         */
		double getDouble(const string& key, const function<bool(double)> &predicate, string error) const;

		/**
		 * Returns a bool value associated with the supplied key. The method is able to
		 * recognize a case-insensitive t/true and f/false strings. Otherwise, throws exception.
         * @param key key to be searched for
		 * @throws InvalidFieldException when there is no such field in the configuration file or
		 *	it is not recognized as a bool value
         * @return a bool value found in config
         */
		bool getBool(const string& key) const;

		/*
		 * Used to notify that there was a field which couldn't be found or parsed
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

