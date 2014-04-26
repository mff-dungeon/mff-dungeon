/* 
 * Common class for drivers.
 */

#ifndef DRIVER_HPP
#define	DRIVER_HPP

#include "common.hpp"
#include "ActionQueue.hpp"
#include "Objects/Alive.hpp"

namespace Dungeon {

    class Driver {
    public:
        Driver(ActionQueue* queue, Alive* figure);
        virtual void worker() = 0;

    protected:
        ActionQueue* queue;
        Alive* figure;
    };
}

#endif

