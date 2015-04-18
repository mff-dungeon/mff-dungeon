#include "RegexMatcher.hpp"

namespace Dungeon {

    bool RegexMatcher::match(const string& reg, const string& text) {
		return regex_match(text, regex(reg));
    }

    bool RegexMatcher::match(const string& reg, const string& text, smatch& matches) {
		return regex_match(text, matches, regex(reg));
    }

    function<bool (string)> RegexMatcher::matcher(string reg) {
        return [reg] (string text) {
            return RegexMatcher::match(reg, text);
        };
    }

    function<bool (string)> CaptureMatcher::matcher(string reg) {
        return [reg, this] (string text) {
            return this->match(reg, text);
        };
    }
}