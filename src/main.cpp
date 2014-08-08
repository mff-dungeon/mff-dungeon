#include "common.hpp"
#include "dynamic.hpp"
#include "GameManager.hpp"
#include "ActionQueue.hpp"
#include "JabberDriver.hpp"
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
JabberDriver* jabber;

/* 
 *	Sends stop signal to the ActionQueue
 */
void finish(int signal) {
    LOGH("Finish");
    
	if(signal == SIGINT) LOG("main") << "Caught SIGINT, terminating..." << LOGF;
	else if(signal == SIGTERM) LOG("main") << "Caught SIGTERM, terminating..." << LOGF;
	
    jabber->stop();
	aqueue->stop();
}

/*
 *	Starts the dungeon and waits until it finishes. Registers signals so
 *		the queue can be stopped.
 */
void start() {
    LOGH("Start");
	LOG("main") << "This is Jabber Dungeon starting." << LOGF;
	gm = new GameManager();
	aqueue = new ActionQueue (gm);
	
	admin = new Alive("human/admin@jabberdung");
    
	jabber = new JabberDriver(aqueue, admin, "dungeon@eideo.cz", "somemagicshit");
	jabber->run();
    LOG("main") << "Jabber driver started, starting queue." << LOGF;
	 
	signal(SIGTERM, finish);
	signal(SIGINT, finish);
	aqueue->loopToFinish();
	
	LOG("main") << "Dungeon ends. Bye!" << LOGF;
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
	LOG("main") << "Database cleanup requested. Are you sure? [y/N]" << LOGF;

	std::getline(std::cin, answer);
	if(answer != "y") { 
		LOG("main") << "Cleanup canceled." << LOGF;
	}
	else {
		LOG("main") << "Cleanup initiated." << LOGF;
		LOG("main") << "Dropping all tables." << std:: endl;
		DatabaseHandler::getInstance().dropDatabase();
		LOG("main") << "All tables dropped, creating and initializing tables." << LOGF;
		DatabaseHandler::getInstance().initDatabase();
		LOG("main") << "Initialization completed." << LOGF;
	}
	LOG("main") << "Continue with startup? [Y/n]" << LOGF;
	std::getline(std::cin, answer);
	if(answer == "n") {
		LOG("main") << "Shutting down" << LOGF;
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
			LOG("main") << "Unknown argument: " << argv[a];
	}
	/*
	 *	Regular dungeon start 
	 */
	LOG("main") << "Starting dungeon..." << LOGF;
	start();
	return 0;
}

