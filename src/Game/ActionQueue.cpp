#include "ActionQueue.hpp"
#include "../Drivers/Driver.hpp"
#include <algorithm>

namespace Dungeon {

	ActionQueue::ActionQueue(GameManager* gm) {
		this->gm = gm;
	}

	void ActionQueue::enqueue(ActionDescriptor* ad) {
		lock l(q_mutex);
		ad->enqueued(this->gm);
		LOGS("ActionQueue", Debug) << "Enqueued an action requested by user." << LOGF;
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

		ActionDescriptor *ad = actions.front();
		LOGS("ActionQueue", Verbose) << "Processing action \"" << ad->in_msg << "\"." << LOGF;
		gm->roundBegin(ad);
		actions.pop();

		bool flawless = false;
		try {
			ad->driver->processDescriptor(ad);

			// Record interaction
			// FIXME uninitialised caller -> memory error
			ad->getCaller()->markInteraction()->save();

			flawless = true;
		} catch (GameException& ge) {
			LOGS("ActionQueue", Error) << "Game exception occured and Driver missed it. " << ge.what() << LOGF;
		}
		/* Disabled for debugging, enable on production
		catch (char const * e) {
			LOGS("ActionQueue", Error) << e << LOGF;			
		}
		catch (int e) {
			LOGS("ActionQueue", Error) << "Exception number " << e << " has been thrown while processing." << LOGF;	
		}
		catch (...) {
			LOGS("ActionQueue", Error) << "Unknown error has occured while processing." << LOGF;	
		}*/

		gm->roundEnd(flawless);
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

}


