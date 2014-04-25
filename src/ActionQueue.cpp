/* 
 * File:   ActionQueue.cpp
 * Author: aearsis
 * 
 * Created on 25. duben 2014, 13:34
 */

#include "ActionQueue.hpp"
#include "ActionDescriptor.hpp"

namespace Dungeon {

	ActionQueue::ActionQueue(GameManager* gm) {
		this->gm = gm;
	}

	ActionDescriptor* ActionQueue::enqueue(Action* action) {
		ActionDescriptor *ad = new ActionDescriptor(action, this->gm);
		if (running) this->actions.push(ad);
		return ad;
	}

	void ActionQueue::process() {
		if (actions.empty()) return;
		
		ActionDescriptor *ad = actions.front();
		ad->getAction()->commit(ad);
	}

	void ActionQueue::loopToFinish() {
		while (running) {
			process();
		}
	}


}


