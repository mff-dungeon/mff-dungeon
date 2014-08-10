#include "TextDriver.hpp"

namespace Dungeon {
    
    TextDriver::TextDriver(ActionQueue* queue) : Driver(queue) {
        this->alist = new ActionList();
    }

    TextDriver::~TextDriver() {
		for (size_t i = 0; i < alist->size(); i++) {
            delete alist->at(i);
		}
        
		delete alist;
    }
    
    bool TextDriver::process(string input, objId figureId) {
        alist->clear();
        
        // TODO: find all actions without actually loading Alive*
        // figure->getAllActions(alist);
        
        for (size_t i = 0; i < alist->size(); i++) {
            Action *action = alist->at(i);
            
            if (action->matchCommand(input)) {
                queue->enqueue(action, figureId);
                
                return true;
            }
        }
        
        return false;
    }
    
    string TextDriver::getDontUnderstandResponse(string input) {
        stringstream ss;
        
        // TODO: think of something more creative
        
        int rnd = rand() % 5;
        switch (rnd) {
            case 0:
                ss << "What do you mean \"" << input << "\"?";
                break;
            case 1:
                ss << "The day may come when I understand \"" << input << "\" but it's not this day.";
                break;
            case 2:
                ss << "You just don't get it, do you? I do not understand \"" << input << "\"!";
                break;
            case 3:
                ss << "One does not simply ask me to \"" << input << "\".";
                break;
            case 4:
                ss << "I wouldn't do that if I were you. Besides, I can't do that.";
                break;
        }
        
        return ss.str();
    }
}