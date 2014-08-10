#include "ConsoleDriver.hpp"

namespace Dungeon {
    ConsoleDriver::ConsoleDriver(ActionQueue* queue, Alive* figure) : TextDriver(queue), figure(figure) {
        
    }

    ConsoleDriver::~ConsoleDriver() {
    }
    
    void ConsoleDriver::worker() {
        LOG("ConsoleDriver") << "Worker started." << LOGF;
        
        string line;
        while(getline(cin, line), !cin.eof()) {
            if (!this->process(line, figure->getId())) {
                cout << this->getDontUnderstandResponse(line) << endl;
            }
        }
        
        LOG("ConsoleDriver") << "Worker ended." << LOGF;
        this->queue->stop();
    }
    
    void ConsoleDriver::run() {
        // create thread with worker()
        this->workerThread = new thread(&ConsoleDriver::worker, this);
    }
}
