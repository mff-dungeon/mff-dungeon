/* 
 * A little bit magic.
 * Every thinkable regex callback function in one file.
 */

#ifndef REGEXMATCHER_CPP
#define	REGEXMATCHER_CPP

#include <regex>
#include "common.hpp"

namespace Dungeon {
    
    template <char const * T>
    class RegexMatcher {
    public:
        static bool match (string text);
    };
    
    template<char const * T>
    bool RegexMatcher<T>::match(string text) {
            return regex_match(text, regex(T));
    }
}

#endif

