#include "TextDriver.hpp"

namespace Dungeon {
    
    TextDriver::TextDriver(ActionQueue* queue, Alive* figure) : Driver(queue, figure) {
        this->alist = new ActionList();
    }

    TextDriver::~TextDriver() {
		//TODO: check what is alist, delete[] instead
		delete alist;
    }
    
    string TextDriver::process(string input) {
        /*
         Get my actions, find if any of them match, prepare result
         */
        string s = "Processing " + input + "\n";
        return s;
    }
}