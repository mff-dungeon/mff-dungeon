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
		LOGS(Debug) << "Enqueued an action requested by user." << LOGF;
		bool wake = actions.empty();
		if (running) this->actions.push(ad);
		if (wake) q_condvar.notify_one();
	}

	void ActionQueue::process() {
		ActionDescriptor *ad;
		{
			ulock u(q_mutex);
			while (actions.empty() && running) q_condvar.wait(u);
			if (actions.empty()) return;

			ad = actions.front();
			actions.pop();
		}

		LOGS(Verbose) << "Processing action \"" << ad->in_msg << "\"." << LOGF;
		gm->roundBegin(ad);

		bool flawless = false;
		try {
			ad->driver->processDescriptor(ad);

			flawless = true;
		} catch (GameException& ge) {
			LOGS(Error) << "Game exception occured and Driver missed it. " << ge.what() << LOGF;
		}
		catch (const std::exception& e) {
			LOGS(Error) << e.what() << LOGF;
		}
		catch (char const * e) {
			LOGS(Error) << e << LOGF;
		}
		catch (int e) {
			LOGS(Error) << "Exception number " << e << " has been thrown while processing." << LOGF;
		}
		catch (...) {
			LOGS(Error) << "Unknown error has occured while processing." << LOGF;
		}

		gm->roundEnd(flawless);
	}

	void ActionQueue::loopToFinish() {
		LOG << "Started." << LOGF;
		while (running) {
			process();
		}
	}

	void ActionQueue::stop() {
		if (!this->running) {
			return;
		}

		this->running = false;
		LOG << "Stopped." << LOGF;
		q_condvar.notify_one();
	}

}


