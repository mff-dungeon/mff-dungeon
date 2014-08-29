#include "ActionQueue.hpp"
#include "Driver.hpp"
#include "Objects/Alive.hpp"
#include <algorithm>

namespace Dungeon {

	ActionQueue::ActionQueue(GameManager* gm) {
		this->gm = gm;
	}

	void ActionQueue::enqueue(ActionDescriptor* ad) {
		lock l(q_mutex);
		ad->enqueued(this->gm);
		bool wake = actions.empty();
		if (running) this->actions.push(ad);
		if (wake) q_condvar.notify_one();
	}

	void ActionQueue::process() {
		ulock u(q_mutex);
		while (actions.empty() && running) {
			q_condvar.wait(u);
                }
		if (actions.empty()) return;

        LOGS("ActionQueue", Verbose) << "Processing action." << LOGF;
		ActionDescriptor *ad = actions.front();
		actions.pop();

		ad->driver->processDescriptor(ad);
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


