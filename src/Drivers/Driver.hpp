#ifndef DRIVER_HPP
#define	DRIVER_HPP

#include "../common.hpp"

namespace Dungeon {

        class ActionQueue;
        class ActionDescriptor;
        
	/* 
	 * Common class for drivers
	 */
	class Driver {
	public:
		Driver(ActionQueue* queue);
		virtual ~Driver() { }
                
                Driver& operator=(const Driver& right)
                {
                    if (this == &right)
                        return *this;
                    throw logic_error("Driver cannot be copied.");
                }
                

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

