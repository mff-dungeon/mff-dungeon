/* 
 * Matcher returns a function for specified regex.
 * @see CallbackAction for usage.
 */

#ifndef REGEXMATCHER_HPP
#define	REGEXMATCHER_HPP

#include <regex>
#include "../common.hpp"

namespace Dungeon {
    
    class RegexMatcher {
    public:
        static bool match (const string& reg, const string& text);
        static bool match (const string& reg, const string& text, smatch& matches);
        static function<bool (string)> matcher(string reg);
    };
}

#endif

