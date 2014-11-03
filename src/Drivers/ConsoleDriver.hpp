#ifndef CONSOLEDRIVER_HPP
#define	CONSOLEDRIVER_HPP

#include "../common.hpp"
#include "../Drivers/TextDriver.hpp"
#include "../Objects/Human.hpp"
#include <thread>

namespace Dungeon {

	/**
	 * Takes commands from stdin, outputs replies to stdout
	 */
	class ConsoleDriver : public TextDriver {
	public:
		ConsoleDriver(ActionQueue* queue, Human* figure);
		virtual ~ConsoleDriver();
		void run();

		virtual void processDescriptor(ActionDescriptor* ad);

	private:
		void worker();
		thread *workerThread;
		Human* figure;
	};

}

#endif	/* CONSOLEDRIVER_HPP */

