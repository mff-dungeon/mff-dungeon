#include "common.hpp"
#include "dynamic.hpp"
#include "GameManager.hpp"
#include "ActionQueue.hpp"
#include "ConsoleDriver.hpp"
#include "DatabaseHandler.hpp"
#include <iostream>
#include <signal.h>		
#include <cstring>		// strcmp

using namespace Dungeon;

/*
 *	Global variables - global in order to be accessable by all functions 
 *		(mainly because of aqueue by finish() )  
 */
GameManager *gm;
ActionQueue* aqueue;
Alive* admin;
ConsoleDriver* console;
//XmppListener* xmpp;

/* 
 *	Sends stop signal to the ActionQueue
 */
void finish(int signal) {	
	if(signal == SIGINT) std::cout << " Caught SIGINT, terminating..." << std::endl;
	else if(signal == SIGTERM) std::cout << " Caught SIGTERM, terminating..." << std::endl;
	
	aqueue->stop();
	/*
	xmpp.stopThread();
	xmpp.disconnect();
	*/
}

/*
 *	Starts the dungeon and waits until it finishes. Registers signals so
 *		the queue can be stopped.
 */
void start() {
	std::cout << "This is Jabber Dungeon starting." << std::endl;
	gm = new GameManager();
	aqueue = new ActionQueue (gm);
	
	admin = new Alive("human/admin@jabberdung");
    
	console = new ConsoleDriver(aqueue, admin);
	console->run();
    std::cout << "Console driver started, starting queue." << std::endl;    
	//XmppListener* xmpp = new XmppListener ();
	//xmpp->run();
	 
	signal(SIGTERM, finish);
	signal(SIGINT, finish);
	aqueue->loopToFinish();
	
	std::cout << "Dungeon ends. Bye!" << std::endl;
}

/*
 *	Function handling DB restart. Should implement all the cleanup procedures.
 *	Basic cleanup function now found in DatabaseHandler::drop() and init()
 * 
 *	Returns 1 if the user demands to terminate the program after the cleanup,
 *	zero otherwise.
 */
int dbRestart() {
	std::string answer;
	std::cout << "Database cleanup requested. Are you sure? [y/N]" << std::endl;

	std::getline(std::cin, answer);
	if(answer != "y") { 
		std::cout << "Cleanup canceled." << std::endl;
	}
	else {
		std::cout << "Cleanup initiated." << std::endl;
		std::cout << "Dropping all tables." << std:: endl;
		DatabaseHandler::getInstance().dropDatabase();
		std::cout << "All tables dropped, creating and initializing tables." << std::endl;
		DatabaseHandler::getInstance().initDatabase();
		std::cout << "Initialization completed." << std::endl;
	}
	std::cout << "Continue with startup? [Y/n]" << std::endl;
	std::getline(std::cin, answer);
	if(answer == "n") {
		std::cout << "Shutting down" << std::endl;
		return 1;
	}
	return 0;
}

int main(int argc, char** argv) {
	/*
	 *	Process command line parameters, if any found
	 */
	for(int a=1; a<argc; a++) {
		if(strcmp(argv[a], "cleanDB") == 0) {
			if(dbRestart() == 1)	// Terminate the program, don't start
				return 0;
		}
		else 
			cout << "Unknown argument: " << argv[a] << std::endl;
	}
	/*
	 *	Regular dungeon start 
	 */
	std::cout << "Starting dungeon..." << std::endl;
	start();
	return 0;
}

