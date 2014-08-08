/*  
 * File:   ActionQueue.cpp
 * Author: aearsis
 * 
 * Created on 25. duben 2014, 13:34
 */

#include "ActionQueue.hpp"
#include "Driver.hpp"

namespace Dungeon {

	ActionQueue::ActionQueue(GameManager* gm) {
		this->gm = gm;
	}

	ActionDescriptor* ActionQueue::enqueue(Action* action) {
		lock l(q_mutex);
		ActionDescriptor *ad = new ActionDescriptor(action, this->gm);
		bool wake = actions.empty();
		if (running) this->actions.push(ad);
		if (wake) q_condvar.notify_one();
		return ad;
	}

	void ActionQueue::process() {
		ulock u(q_mutex);
		while (actions.empty() && running) {
			q_condvar.wait(u);
                }
		if (actions.empty()) return;

        LOG("ActionQueue") << "Processing." << LOGF;
		ActionDescriptor *ad = actions.front();
		actions.pop();

		ad->getAction()->commit(ad);
        
        // notify drivers
        for_each(drivers.begin(), drivers.end(), bind2nd(mem_fun(&Driver::processDescriptor), ad));
	}

	void ActionQueue::loopToFinish() {
        LOG("ActionQueue") << "Started." << LOGF;
		while (running) { 
			process();
		}
	}

    void ActionQueue::stop() {
        if (!this->running) {
            return;
        }
        
        this->running = false;
		/* TODO: check - how locks are implemented? can't notify_one 
		 * in the middle of action processing start another processing?
		 * Remove if not true, needed for safe finishing
		 */ 
		LOG("ActionQueue") << "Stopped." << LOGF;
        q_condvar.notify_one();
    }
    
    void ActionQueue::registerDriver(Driver *driver) {
        drivers.push_back(driver);
    }
    
    void ActionQueue::unregisterDriver(Driver *driver) {
        drivers.erase(remove(drivers.begin(), drivers.end(), driver), drivers.end());
    }
}


