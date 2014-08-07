#ifndef CONSOLEDRIVER_HPP
#define	CONSOLEDRIVER_HPP

#include "TextDriver.hpp"
#include "Objects/Alive.hpp"
#include "common.hpp"
#include <thread>

namespace Dungeon {
    
    class ConsoleDriver : public TextDriver {
    public:
        ConsoleDriver(ActionQueue* queue, Alive* figure);
        virtual ~ConsoleDriver();
        void run();
        
    private:
        void worker();
        thread *workerThread;
        Alive* figure;
    };

}

#endif	/* CONSOLEDRIVER_HPP */

