#include "Config.hpp"

namespace Dungeon {
	
	bool Config::initialized = false;
	string Config::p_dbName = "dungeon.db";
	
	void Config::loadFile(const string& fileName) {
		ConfigParser cp(fileName);

		Config::p_dbName = cp.getString("DB_NAME");

		initialized = true;
	}

}
