#include "ConsoleDriver.hpp"

namespace Dungeon {
    ConsoleDriver::ConsoleDriver(ActionQueue* queue, Alive* figure) : TextDriver(queue), figure(figure) {
        
    }

    ConsoleDriver::~ConsoleDriver() {
    }
    
    void ConsoleDriver::worker() {
        // do some shit
        cout << "[ CD ] Worker started.\n";
        
        string line, output;
        while(getline(cin, line), !cin.eof()) {
            output = this->process(line, figure);
            cout << output;
        }
        
        cout << "[ CD ] Worker ended." << endl;
        this->queue->stop();
    }
    
    void ConsoleDriver::run() {
        // create thread with worker()
        this->workerThread = new thread(&ConsoleDriver::worker, this);
    }
}
