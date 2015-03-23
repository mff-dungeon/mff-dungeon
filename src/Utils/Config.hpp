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
		
		static const string& DbName() {	return p_dbName; }
		static const string& JabberName() {	return p_jabberName; }
		static const string& JabberPasswd() { return p_jabberPasswd; }
		static const int GameMode() { return p_gameMode; }
		static const int RespawnTime() { return p_respawnTime; }
		static const int LevelStats() { return p_levelStats; }
		
		static const int EnemyHp() { return p_enemyHp; }
		static const int EnemyRespawn() { return p_enemyRespawn; }
		static const int EnemyMatsDrop() { return p_enemyMatsDrop; }
		static const int EnemyBossKey() { return p_enemyBossKey; }
		static const int RecipeExp() { return p_recipeExp; }
		static const int RecipeMat() { return p_recipeMat; }
		static const int RecipeKeyMat() { return p_recipeKeyMat; }
		
	private:
		Config() {};
		Config(Config const&) = delete;
		~Config() {};
		void operator=(Config const&) = delete;
		
		// This method should be updated when new fiels are added
		static void loadFile(const string& fileName);
		
		static bool initialized;
		
		static string p_dbName;
		static string p_jabberName;
		static string p_jabberPasswd;
		static int p_gameMode;
		static int p_respawnTime;
		static int p_levelStats;
		
		static int p_enemyHp;
		static int p_enemyRespawn;
		static int p_enemyMatsDrop;
		static int p_enemyBossKey;
		static int p_recipeExp;
		static int p_recipeMat;
		static int p_recipeKeyMat;
	};
}
#endif	/* CONFIG_HPP */

