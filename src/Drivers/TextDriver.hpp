#ifndef TEXTDRIVER_HPP
#define	TEXTDRIVER_HPP

#include <sstream>
#include <cstdlib>
#include "../common.hpp"
#include "Driver.hpp"
#include "../Actions/Action.hpp"
#include "../Game/ActionQueue.hpp"
#include "../Game/ActionList.hpp"

namespace Dungeon {

	/**
	 * Common class for text-communicating drivers.
	 * Matches actions to strings, replies in string.
	 */
	class TextDriver : public Driver {
	public:
		TextDriver(ActionQueue* queue) : Driver(queue) { }
		virtual ~TextDriver() { }

		bool process(TextActionDescriptor* ad);
		void processFill(TextActionDescriptor* ad);
		void processMatch(TextActionDescriptor* ad);
		void processUserReply(TextActionDescriptor* ad);
		void processRun(TextActionDescriptor* ad);

                /**
                 * Get some fancy response to unmatchable input
                 * @param input the input we cannot match
                 * @param timeSinceLastMatched number of unmatched attempts since last matched attempt
                 * @return text response
                 */
		string getDontUnderstandResponse(string input, int timeSinceLastMatched);

                /**
                 * Called for all new users
                 * @return action for user's first interaction
                 */
		Action* getCreateAction();
	protected:
		map<objId, TextActionDescriptor*> dialogs;
		bool isInDialog(objId who) {
			return dialogs.find(who) != dialogs.end();
		}
                
                map<objId, int> matchingPatience;
                int getPatience(objId who) {
                    auto it = matchingPatience.find(who);
                    return it != matchingPatience.end() ? it->second : 0;
                }
                
                void incrementPatience(objId who) {
                    auto it = matchingPatience.find(who);
                    if (it != matchingPatience.end()) {
                        it->second++;
                    } else {
                        matchingPatience.insert(pair<objId, int>(who, 1));
                    }
                }
                
                void resetPatience(objId who) {
                    matchingPatience.erase(who);
                }
	};
}
#endif

