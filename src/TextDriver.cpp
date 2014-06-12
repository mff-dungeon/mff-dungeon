#include "TextDriver.hpp"

namespace Dungeon {
    
    TextDriver::TextDriver(ActionQueue* queue, Alive* figure) : Driver(queue, figure) {
        this->alist = new ActionList();
    }

    TextDriver::~TextDriver() {
		for (size_t i = 0; i < alist->size(); i++) {
		        delete alist->at(i);
		}
        
		delete alist;
    }
    
    string TextDriver::process(string input) {
        alist->clear();
        figure->getAllActions(alist);
        
        string response = "What do you mean \"" + input + "\"?\n";
        for (size_t i = 0; i < alist->size(); i++) {
            Action *action = alist->at(i);
            
            if (action->matchCommand(input)) {
                queue->enqueue(action);
                
                // TODO: connect the action and its proccessing, 
				// so we can show a response
				response = "Found some action, let's see what happens!" << std::endl;
                break;
            }
        }
        
        return response;
    }
}