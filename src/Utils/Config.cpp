#include "Config.hpp"

namespace Dungeon {
	
	bool Config::initialized = false;
	// Default initialized values, must be set for all static fields
	string Config::p_dbName = "dungeon.db";
	string Config::p_jabberName = "dungeon@eideo.cz";
	string Config::p_jabberPasswd = "********";
	int Config::p_gameMode = 0;
	
	void Config::loadFile(const string& fileName) {
		ConfigParser cp(fileName);

		Config::p_dbName = cp.getString("DB_NAME");
		Config::p_jabberName = cp.getString("JABBER_NAME", [](string s) -> bool {return s.find("@") != string::npos; }, "The jabber name specified is invalid.");
		Config::p_jabberPasswd = cp.getString("JABBER_PASSWD");
		Config::p_gameMode = cp.getInt("GAME_MODE", [](int i) -> bool {return i == 0 || i == 1; }, "The game mode must be set either to 0 or 1.");

		initialized = true;
	}

}
