#include "FuzzyStringMatcher.hpp"

namespace Dungeon {
	
    vector<string> StringMatcher::tokenize(const string& instr) {
		if (instr.length() == 0) {
			LOGS("FSM", Error) << "Tried to tokenize empty string." << LOGF;
			return vector<string>(0);
		}
        char * str = strdup(instr.c_str());
        char * ptr;
        vector<string> ret;
		ptr = strtok(str, " .,:()?!");
		if (ptr == nullptr) {
			ret.push_back(instr);
		} else do {
			ret.push_back(ptr);
		} while ((ptr = strtok(NULL, " .,:()?!")));
        free(str);
        return ret;
    }
}

