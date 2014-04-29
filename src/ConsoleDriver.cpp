#include "ConsoleDriver.hpp"

namespace Dungeon {
    ConsoleDriver::ConsoleDriver(ActionQueue* queue, Alive* figure) : Driver(queue, figure) {
        
    }

    ConsoleDriver::~ConsoleDriver() {
    }
    
    void ConsoleDriver::worker() {
        // do some shit
        cout << "[ CD ] Worker started" << endl;
        
        string line;
        while(cin >> line, !cin.eof()) {
            cout << line << endl;
        }
        
        this->queue->stop();
        cout << "[ CD ] Worker ended" << endl;
    }
    
    void ConsoleDriver::run() {
        // create thread with worker()
        this->workerThread = new thread(&ConsoleDriver::worker, this);
    }
}
