#include "TextDriver.hpp"

namespace Dungeon {
    
    TextDriver::TextDriver(ActionQueue* queue, Alive* figure) : Driver(queue, figure) {
        
    }

    TextDriver::~TextDriver() {
    }
    
    string TextDriver::process(string input) {
        ActionList* alist = new ActionList();
        /*
         Get my actions, find if any of them match, prepare result
         */
        string s = "Processing " + input + "\n";
        delete alist;
        return s;
    }
}