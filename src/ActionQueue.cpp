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

		ActionDescriptor *ad = actions.front();
		actions.pop();

		ad->getAction()->commit(ad);
	}

	void ActionQueue::loopToFinish() {
		while (running) {
                        cout << "[ AQ ] Processing" << endl; 
			process();
		}
	}

        void ActionQueue::stop() {
            this->running = false;
            q_condvar.notify_one();
        }
}


