#include "Config.hpp"

namespace Dungeon {
	
	bool Config::initialized = false;
	// Default initialized values, must be set for all static fields
	string Config::p_dbName = "dungeon.db";
	string Config::p_jabberName = "dungeon@eideo.cz";
	string Config::p_jabberPasswd = "********";
	int Config::p_gameMode = 0;
	int Config::p_respawnTime = 120;
	int Config::p_levelStats = 5;
	
	void Config::loadFile(const string& fileName) {
		ConfigParser cp(fileName);

		Config::p_dbName = cp.getString("DB_NAME");
		Config::p_jabberName = cp.getString("JABBER_NAME", [](string s) -> bool {return s.find("@") != string::npos; }, "The jabber name specified is invalid.");
		Config::p_jabberPasswd = cp.getString("JABBER_PASSWD");
		Config::p_gameMode = cp.getInt("GAME_MODE", [](int i) -> bool {return i == 0 || i == 1; }, "The game mode must be set either to 0 or 1.");
		Config::p_respawnTime = cp.getInt("RESPAWN_INTERVAL", [](int i) -> bool {return i >= 0 && i <= 604800; }, "The respawn time must be in interval (0, 604800).");
		Config::p_levelStats = cp.getInt("LEVEL_STAT_POINTS", [](int i) -> bool {return i > 0 && i <= 1000; }, "The level up stat points must be set to a value between 1 and 1000.");

		initialized = true;
	}

}
