#ifndef TEXTDRIVER_HPP
#define	TEXTDRIVER_HPP

#include <sstream>
#include <cstdlib>
#include "common.hpp"
#include "Driver.hpp"
#include "Action.hpp"
#include "ActionQueue.hpp"
#include "Objects/Alive.hpp"

namespace Dungeon {
    
    class TextDriver : public Driver {
    public:
        TextDriver(ActionQueue* queue);
        virtual ~TextDriver();
        
        bool process(string input, objId figureId);
        string getDontUnderstandResponse(string input);
    private:
	ActionList* alist;
    };
}
#endif

