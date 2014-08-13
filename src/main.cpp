#include <iostream>
#include <sstream>
#include <signal.h>		
#include <cstring>		// strcmp
#include <execinfo.h>
#include <unistd.h>
#include "common.hpp"
#include "dynamic.hpp"
#include "GameManager.hpp"
#include "ActionQueue.hpp"
#include "JabberDriver.hpp"
#include "DatabaseHandler.hpp"
#include "ConsoleDriver.hpp"

using namespace Dungeon;

/*
 *	Global variables - global in order to be accessable by all functions 
 *		(mainly because of aqueue by finish() )  
 */
GameManager *gm;
Alive* admin;
JabberDriver* jabber;
ConsoleDriver* console;

/*
 *  Attempts to get current backtrace
 */
string getStacktace() {
    void* tracePtrs[100];
    int count = backtrace(tracePtrs, 100);
    
    char** funcNames = backtrace_symbols(tracePtrs, count);
    stringstream ss;
    
    // Print the stack trace
    for (int i = 0; i < count; i++) {
        string line(funcNames[i]);
        ss << endl << '\t' << line;
    }
    
    // Free the string pointers
    free(funcNames);
    
    return ss.str();
}

/* 
 *	Sends stop signal to the ActionQueue
 */
void finish(int signal) {
	LOGH("Finish");

	if (signal == SIGINT) LOG("main") << "Caught SIGINT, terminating..." << LOGF;
	else if (signal == SIGTERM) LOG("main") << "Caught SIGTERM, terminating..." << LOGF;

	jabber->stop();
	gm->shutdown();
}

/*
 *  Handles segfault.
 */
void crash_segv(int signal) {
    LOGS("main", Fatal) << "Caught SIGSEGV (" << signal << "), crashing..." << getStacktace() << LOGF;
    exit(signal);
}

/*
 *	Starts the dungeon and waits until it finishes. Registers signals so
 *		the queue can be stopped.
 */
void start() {
	LOGH("Start");
	LOG("main") << "This is Jabber Dungeon starting." << LOGF;
	gm = new GameManager();

	admin = new Alive("human/admin@jabberdung");
	console = new ConsoleDriver(gm->getQueue(), admin);
	console->run();

	jabber = new JabberDriver(gm, "dungeon@eideo.cz", "somemagicshit");
	jabber->run();
	LOG("main") << "Drivers started, starting queue." << LOGF;

	signal(SIGTERM, finish);
	signal(SIGINT, finish);
    signal(SIGSEGV, crash_segv);
	gm->getQueue()->loopToFinish();

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
	string answer;
	LOG("main") << "Database cleanup requested. Are you sure? [y/N]" << LOGF;

	getline(cin, answer);
	if (answer != "y") {
		LOG("main") << "Cleanup canceled." << LOGF;
	} else {
		LOG("main") << "Cleanup initiated." << LOGF;
		LOG("main") << "Dropping all tables." << LOGF;
		DatabaseHandler::getInstance().dropDatabase();
		LOG("main") << "All tables dropped, creating and initializing tables." << LOGF;
		DatabaseHandler::getInstance().initDatabase();
		LOG("main") << "Initialization completed." << LOGF;
	}
	LOG("main") << "Continue with startup? [Y/n]" << LOGF;
	getline(cin, answer);
	if (answer == "n") {
		LOG("main") << "Shutting down" << LOGF;
		return 1;
	}
	return 0;
}

int main(int argc, char** argv) {
    Logger::initialize();
    
	/*
	 *	Process command line parameters, if any found
	 */
	for (int a = 1; a < argc; a++) {
		if (strcmp(argv[a], "cleanDB") == 0) {
			if (dbRestart() == 1) // Terminate the program, don't start
				return 0;
		} else
			LOG("main") << "Unknown argument: " << argv[a];
	}
	/*
	 *	Regular dungeon start 
	 */
	LOG("main") << "Starting dungeon..." << LOGF;
	start();
	return 0;
}

