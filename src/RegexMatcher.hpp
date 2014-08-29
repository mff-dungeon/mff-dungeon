/* 
 * Matcher returns a function for specified regex.
 * @see CallbackAction for usage.
 */

#ifndef REGEXMATCHER_HPP
#define	REGEXMATCHER_HPP

#include <regex>
#include "common.hpp"

namespace Dungeon {
    
    class RegexMatcher {
    public:
        static bool match (string reg, string text);
        static bool match (string reg, string text, smatch& matches);
        static function<bool (string)> matcher(string reg);
    };
}

#endif

