#ifndef TEXTDRIVER_HPP
#define	TEXTDRIVER_HPP

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
        
        string process(string input, Alive* figure);
    private:
		ActionList* alist;
    };
}
#endif

