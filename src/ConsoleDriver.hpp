#include "TextDriver.hpp"
#include "common.hpp"
#include <thread>

#ifndef CONSOLEDRIVER_HPP
#define	CONSOLEDRIVER_HPP

namespace Dungeon {
    
    class ConsoleDriver : public TextDriver {
    public:
        ConsoleDriver(ActionQueue* queue, Alive* figure);
        virtual ~ConsoleDriver();
        void run();
        
    private:
        void worker();
        thread *workerThread;
    };

}

#endif	/* CONSOLEDRIVER_HPP */

