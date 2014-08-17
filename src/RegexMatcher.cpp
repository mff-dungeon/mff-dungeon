#include "RegexMatcher.hpp"

namespace Dungeon {
    
    bool RegexMatcher::match(string reg, string text) {
		return regex_match(text, regex(reg));
    }
    
    function<bool (string)> RegexMatcher::matcher(string reg) {
        return [reg] (string text) {
            return RegexMatcher::match(reg, text);
        };
    }
}