/* 
 * Common class for drivers.
 */

#ifndef DRIVER_HPP
#define	DRIVER_HPP

#include "common.hpp"
#include "ActionQueue.hpp"

namespace Dungeon {

    class Driver {
    public:
        Driver(ActionQueue* queue);

    protected:
        ActionQueue* queue;
    };
}

#endif

