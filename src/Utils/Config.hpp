#ifndef CONFIG_HPP
#define	CONFIG_HPP

#include <string>
#include "../common.hpp"
#include "ConfigParser.hpp"

namespace Dungeon {
	class Config {
	public:
		/**
		 * Checks if the Config is properly set and loaded
         */
		static bool isInitialized() {
			return initialized;
		}
		
		/**
		 * Loads configuration informating from a file.
		 * May throw a ConfigParser::InvalidFieldException and ConfigParser::InvalidFileException exceptions
         * @param fileName a path to the loaded config file
         */
		static void initialize(const string& fileName) {
			if(!initialized) {
				loadFile(fileName);
			}
		}
		
		/**
		 * Returns set path for the game database.
         */
		static const string& dbName() {
			return p_dbName;
		}
		
	private:
		Config() {};
		Config(Config const&) = delete;
		~Config() {};
		void operator=(Config const&) = delete;
		
		// This method should be updated when new fiels are added
		static void loadFile(const string& fileName);
		
		static bool initialized;
		
		static string p_dbName;
	};
}
#endif	/* CONFIG_HPP */

