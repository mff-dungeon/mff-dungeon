#ifndef CONSOLEDRIVER_HPP
#define	CONSOLEDRIVER_HPP

#include "TextDriver.hpp"
#include "Objects/Alive.hpp"
#include "common.hpp"
#include <thread>

namespace Dungeon {
    
    /**
     * Takes commands from stdin, outputs replies to stdout
     */
    class ConsoleDriver : public TextDriver {
    public:
        ConsoleDriver(ActionQueue* queue, Alive* figure);
        virtual ~ConsoleDriver();
        void run();
        
        virtual void processDescriptor(ActionDescriptor* ad);
        
    private:
        void worker();
        thread *workerThread;
        Alive* figure;
    };

}

#endif	/* CONSOLEDRIVER_HPP */

