#include "ConsoleDriver.hpp"

namespace Dungeon {
    ConsoleDriver::ConsoleDriver(ActionQueue* queue, Alive* figure) : TextDriver(queue), figure(figure) {
        
    }

    ConsoleDriver::~ConsoleDriver() {
    }
    
    void ConsoleDriver::worker() {
        LOG("ConsoleDriver") << "Worker started." << LOGF;
        
        string line, output;
        while(getline(cin, line), !cin.eof()) {
            output = this->process(line, figure);
            LOG("ConsoleDriver") << output;
        }
        
        LOG("ConsoleDriver") << "Worker ended." << LOGF;
        this->queue->stop();
    }
    
    void ConsoleDriver::run() {
        // create thread with worker()
        this->workerThread = new thread(&ConsoleDriver::worker, this);
    }
}
