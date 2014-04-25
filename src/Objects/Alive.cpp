#include "Alive.hpp"

namespace Dungeon {
    
    constexpr char suicideRegex [] = "commit (a )?suicide";

    void Alive::getActions(ActionList* list, IObject *callee) {
        if (callee == 0) {
                // Ask all objects nearby
        }

        // And add some actions "on myself"
        list->push_back(new CallbackAction("suicide", "If you just dont want to live on this planet anymore.",
            RegexMatcher<suicideRegex>::match,
            [this] (ActionDescriptor * ad) {
                    ad->message = "You've killed yaself. Cool yeah?";
                    this->hitpoints = 0;
            }));
    }

}

