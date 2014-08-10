#ifndef CONSOLEDRIVER_HPP
#define	CONSOLEDRIVER_HPP

#include "TextDriver.hpp"
#include "Objects/Alive.hpp"
#include "common.hpp"
#include <thread>

namespace Dungeon {
    
    class ConsoleDriver : public TextDriver {
    public:
        ConsoleDriver(ActionQueue* queue, objId figureId);
        virtual ~ConsoleDriver();
        void run();
        
    private:
        void worker();
        thread *workerThread;
        objId figureId;
    };

}

#endif	/* CONSOLEDRIVER_HPP */

