#ifndef DRIVER_HPP
#define	DRIVER_HPP

#include "common.hpp"
#include "ActionQueue.hpp"
#include "ActionDescriptor.hpp"

namespace Dungeon {
    
    /* 
     * Common class for drivers.
     */
    class Driver {
    public:
        Driver(ActionQueue* queue);
        ~Driver();
        
        /**
         * Called by ActionQueue in it's thread. 
         * Have exclusive access to the world - every other action 
         * is waiting for this method to finish.
         */
        virtual void processDescriptor(ActionDescriptor* descriptor);

    protected:
        ActionQueue* queue;
    };
}

#endif

