#include <string>
#include <vector>
#include <map>
#include <iostream>

#ifndef COMMON_HPP
#define COMMON_HPP

#define DB_NAME "dungeon.db"
#define CONFIG_GAME_MODE 0

/**
 * Relation types used a lot
 */
// Alive is in a room, doors are in the room
#define R_INSIDE "inside"
// Alive has backpack, backpack has potion
#define R_INVENTORY "inventory"
// Door targets somewhere
#define R_TARGET "target"
// Creature drops an item
#define R_DROP "dropper"
// Creature drops an item
#define R_RECIPE "crafter-recipe"
// Something requires a stat
#define R_REQUIREMENT "stat-req"
// Alive has gold
#define R_RESOURCE(type) ( "resource-" + to_string((int)(type)) )

/*
 *  Logging
 */
#include "Logger.hpp"
#define LOG(source) ( Logger::getInstance().beginMessage(source), Logger::getInstance() )
#define LOGS(source, severity) ( Logger::getInstance().beginMessage(source, Logger::Severity::severity), Logger::getInstance() )
#define LOGH(headline) { Logger::getInstance().setHeadline(headline); } 
#define LOGF ( Logger::getInstance().endMessage(), "" )

/*
 *  Random number generation
 */
#include <random>
#define rand_init(var, dist, min, max) std::default_random_engine var(Dungeon::hw_random()); std::uniform_int_distribution<int> dist(min, max);
#define rand_next(var, dist) ( dist(var) )
#define RAND_ALNUM "qwertyuiopasdfghjklzxcvbnm0123456789"
#define RAND_ALNUM_LEN 36
#define RANDID Dungeon::Utils::getRandomString(6)

/*
 *  Helper functions for accessing parts of id
 */
#define objId_getType(id) ( id.substr(0, id.rfind("/")) )
#define objId_getIdentifier(id) ( id.substr(id.rfind("/") + 1) )

using namespace std;

namespace Dungeon {
    /*
     * Circular dependencies...
     */
    class GameManager; 
    class ObjectLoader;
    class ObjectGroup;
    class Action;
    class ActionDescriptor;
    class ActionQueue;
    class Driver;
    class AddIObject;
    class ActionList;
    class Inventory;
    struct ObjectPointer;
    
    class IObject;
    class IDescriptable;
    class Alive;
    class Human;
    class Item;
    class Resource;
    class Potion;
    
    /**
     * Should be value type (struct), comparable
     */
    typedef string objId;
    typedef map<objId, ObjectPointer> ObjectMap;
    typedef map<string, ObjectMap> RelationList;
    
    static random_device hw_random;
    
    class Utils {
    public:
        static string getRandomString(int len) {
            char ret [len + 1];
            ret[len] = 0;
            static rand_init(r, d, 0, RAND_ALNUM_LEN - 1);
            for (int i = 0; i < len; ++i) ret[i] = RAND_ALNUM[rand_next(r, d)];
            return ret;
        }
		
		static int getRandomInt(int min, int max) {
			static rand_init(r, d, min, max);
			return rand_next(r, d);
		}
        
        static string weightStr(const int weight) {
            char buf [50];
            if (weight > 1000000) 
                sprintf(buf, "%.2f t", weight / 1016046.9088);
            else if (weight > 1000) 
                sprintf(buf, "%.2f lb", weight / 453.59237);
            else 
                sprintf(buf, "%.2f oz", weight / 28.349523125);
            return string(buf);
        }
        
        static inline string progressBar(const int val, const int max, const int length) {
            int l = val * length / max;
            char buf [length];
            for (int i = 0; i < length; i++)
                buf[i] = (i <= l ? '#' : '-');
            return string(buf, length);
        }
        
        // courtesy of http://stackoverflow.com/questions/1798112/removing-leading-and-trailing-spaces-from-a-string
	
        // trim from left
        static inline std::string& ltrim(std::string& s, const char* t = " \t\n\r\f") {
            s.erase(0, s.find_first_not_of(t));
            return s;
        }

        // trim from right
        static inline std::string& rtrim(std::string& s, const char* t = " \t\n\r\f") {
            s.erase(s.find_last_not_of(t) + 1);
            return s;
        }

        // trim from left & right
        static inline std::string& trim(std::string& s, const char* t = " \t\n\r\f") {
            return ltrim(rtrim(s, t), t);
        }
        
        static inline std::string& capitalize(std::string&& s, bool all = false) {
            bool cap = true;
            
            for (int i = 0; i <= s.length(); i++) {
                if (isalpha(s[i]) && cap) {
                    s[i] = toupper(s[i]);
                    cap = false;
                    
                    if (!all) break;
                } else if (isspace(s[i])) {
                    cap = true;
                }
            }
            
            return s;
        }
        
        static inline std::string& decapitalize(std::string&& s) {
            for (int i = 0; i <= s.length(); i++) {
                if (isalpha(s[i])) {
                    s[i] = tolower(s[i]);
                }
            }
            
            return s;
        }
    };
    
}
    
#include "exceptions.hpp"
#include "IObject.hpp"
#include "ObjectPointer.hpp"
#include "AddIObject.hpp"


#endif

