#include "common.hpp"
#include "Driver.hpp"
#include "Action.hpp"
#include "ActionQueue.hpp"

#ifndef TEXTDRIVER_HPP
#define	TEXTDRIVER_HPP

namespace Dungeon {
    
    class TextDriver : public Driver {
    public:
        TextDriver(ActionQueue* queue, Alive* figure);
        virtual ~TextDriver();
        
        string process(string input);
    private:
		ActionList* alist;
    };
}
#endif

