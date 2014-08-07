/*  
 * File:   ActionQueue.cpp
 * Author: aearsis
 * 
 * Created on 25. duben 2014, 13:34
 */

#include "ActionQueue.hpp"

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

        cout << "[ AQ ] Processing." << endl;
		ActionDescriptor *ad = actions.front();
		actions.pop();

		ad->getAction()->commit(ad);
	}

	void ActionQueue::loopToFinish() {
        cout << "[ AQ ] Started.\n";
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
		cout << "[ AQ ] Stopped." << endl;
        q_condvar.notify_one();
    }
}


