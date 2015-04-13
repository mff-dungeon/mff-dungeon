#include "Config.hpp"

namespace Dungeon {
	
	bool Config::initialized = false;
	// Default initialized values, must be set for all static fields
	string Config::p_dbName = "dungeon.db";
	string Config::p_jabberName = "dungeon@eideo.cz";
	string Config::p_jabberPasswd = "********";
	int Config::p_gobackStackSize = 10;
	int Config::p_gameMode = 0;
	int Config::p_respawnTime = 120;
	int Config::p_levelStats = 5;
	
	int Config::p_enemyHp = 10;
	int Config::p_enemyRespawn = 10;
	int Config::p_enemyMatsDrop = 5;
	int Config::p_enemyBossKey = 5;
	int Config::p_recipeExp = 2;
	int Config::p_recipeMat = 5;
	int Config::p_recipeKeyMat = 1;
	
	void Config::loadFile(const string& fileName) {
		ConfigParser cp(fileName);

		Config::p_dbName = cp.getString("DB_NAME");
		Config::p_jabberName = cp.getString("JABBER_NAME", [](string s) -> bool {return s.find("@") != string::npos; }, "The jabber name specified is invalid.");
		Config::p_jabberPasswd = cp.getString("JABBER_PASSWD");
		Config::p_gobackStackSize = cp.getInt("GOBACK_STACK_SIZE", [](int i) -> bool {return i >= 2 || i <= 50; }, "The stack size must be a value between 2 and 50.") + 1; // Used for cyclic
		Config::p_gameMode = cp.getInt("GAME_MODE", [](int i) -> bool {return i == 0 || i == 1; }, "The game mode must be set either to 0 or 1.");
		Config::p_respawnTime = cp.getInt("RESPAWN_INTERVAL", [](int i) -> bool {return i >= 0 && i <= 604800; }, "The respawn time must be in interval (0, 604800).");
		Config::p_levelStats = cp.getInt("LEVEL_STAT_POINTS", [](int i) -> bool {return i > 0 && i <= 1000; }, "The level up stat points must be set to a value between 1 and 1000.");
		
		Config::p_enemyHp = cp.getInt("ENEMY_HP_RATE", [](int i) -> bool {return i > 0 && i <= 1000; }, "The enemy hp rate must be set to a value between 1 and 1000.");
		Config::p_enemyRespawn = cp.getInt("ENEMY_RESPAWN_RATE", [](int i) -> bool {return i > 0 && i <= 1000; }, "The enemy respawn rate must be set to a value between 1 and 1000.");
		Config::p_enemyMatsDrop = cp.getInt("ENEMY_MATS_DROP_RATE", [](int i) -> bool {return i > 0 && i <= 1000; }, "The enemy material drop rate must be set to a value between 1 and 1000.");
		Config::p_enemyBossKey = cp.getInt("ENEMY_BOSS_KEY_RATE", [](int i) -> bool {return i > 0 && i <= 1000; }, "The enemy boss key rate must be set to a value between 1 and 1000.");
		Config::p_recipeExp = cp.getInt("RECIPE_EXP_RATE", [](int i) -> bool {return i > 0 && i <= 1000; }, "The recipe exp rate must be set to a value between 1 and 1000.");
		Config::p_recipeMat = cp.getInt("RECIPE_MAT_REQUIRED", [](int i) -> bool {return i > 0 && i <= 1000; }, "The recipe material requirement rate must be set to a value between 1 and 1000.");
		Config::p_recipeKeyMat = cp.getInt("RECIPE_KEY_MAT_REQUIRED", [](int i) -> bool {return i > 0 && i <= 1000; }, "The recipe key material requirement rate must be set to a value between 1 and 1000.");

		initialized = true;
	}
}