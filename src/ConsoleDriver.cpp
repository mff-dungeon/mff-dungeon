#include "ConsoleDriver.hpp"

namespace Dungeon {
    ConsoleDriver::ConsoleDriver(ActionQueue* queue, Alive* figure) : TextDriver(queue, figure) {
        
    }

    ConsoleDriver::~ConsoleDriver() {
    }
    
    void ConsoleDriver::worker() {
        // do some shit
        cout << "[ CD ] Worker started" << endl;
        
        string line, output;
        while(getline(cin, line), !cin.eof()) {
            output = this->process(line);
            cout << output;
        }
        
        this->queue->stop();
        cout << "[ CD ] Worker ended" << endl;
    }
    
    void ConsoleDriver::run() {
        // create thread with worker()
        this->workerThread = new thread(&ConsoleDriver::worker, this);
    }
}
