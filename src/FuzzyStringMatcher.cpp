#include "FuzzyStringMatcher.hpp"

namespace Dungeon {
	
    vector<string> StringMatcher::tokenize(const string& instr) {
        char * str = strdup(instr.c_str());
        char * ptr;
        vector<string> ret;
		ptr = strtok(str, " .,:()?!");
		do {
			ret.push_back(ptr);
		} while ((ptr = strtok(NULL, " .,:()?!")));
        free(str);
        return ret;
    }
}

