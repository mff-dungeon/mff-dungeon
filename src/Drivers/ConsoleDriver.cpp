#include "ConsoleDriver.hpp"

namespace Dungeon {

	ConsoleDriver::ConsoleDriver(ActionQueue* queue, Human* figure) : TextDriver(queue), figure(figure) { }

	void ConsoleDriver::worker() {
		LOG << "Worker started." << LOGF;

		string line;
		while (getline(cin, line), !cin.eof()) {
			TextActionDescriptor * ad = new TextActionDescriptor(this);
			ad->in_msg = line;
			ad->from = "console";

			queue->enqueue(ad);
		}

		LOG << "Worker ended." << LOGF;
		this->queue->stop();
	}

	void ConsoleDriver::processDescriptor(ActionDescriptor* descriptor) {
		TextActionDescriptor* ad = (TextActionDescriptor*) descriptor;
		ad->assigned(figure);

		this->process(ad);

		cout << ">>> " << ad->getReply().plainString() << endl;
	}

	void ConsoleDriver::run() {
		// create thread with worker()
		this->workerThread = new thread(&ConsoleDriver::worker, this);
	}
}
