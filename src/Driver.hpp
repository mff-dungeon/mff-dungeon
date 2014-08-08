/* 
 * Common class for drivers.
 */

#ifndef DRIVER_HPP
#define	DRIVER_HPP

#include "common.hpp"
#include "ActionQueue.hpp"
#include "ActionDescriptor.hpp"

namespace Dungeon {

    class Driver {
    public:
        Driver(ActionQueue* queue);
        ~Driver();
        
        virtual void processDescriptor(ActionDescriptor* descriptor);

    protected:
        ActionQueue* queue;
    };
}

#endif

