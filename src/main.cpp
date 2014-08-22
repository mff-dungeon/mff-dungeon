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
bool initWorld = false;
bool finishing = false;

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
    if (finishing) {
        LOGH("Hard shutdown");
        if (signal == SIGINT) LOG("main") << "Caught another SIGINT, already in shutdown mode." << LOGF;
        else if (signal == SIGTERM) LOG("main") << "Caught another SIGTERM, already in shutdown mode." << LOGF;
        
        LOGS("main", Fatal) << "Resolving potentially uncontrollable hang by voluntary suicide (SIGABRT)...";
        abort();
        return;
    }
    
	LOGH("Finish");
	if (signal == SIGINT) LOG("main") << "Caught SIGINT, terminating..." << LOGF;
	else if (signal == SIGTERM) LOG("main") << "Caught SIGTERM, terminating..." << LOGF;
    
    finishing = true;

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
	gm = new GameManager(initWorld);

	admin = new Alive("human/admin@jabberdung");
	admin->addRelation("inventory", gm->getObjectPointer("ThorsHammer"));
	console = new ConsoleDriver(gm->getQueue(), admin);
	console->run();

	jabber = new JabberDriver(gm, "dungeon@salmelu-i5521", "somemagicshit");
	jabber->run();
	LOG("main") << "Drivers started, starting queue." << LOGF;

	signal(SIGTERM, finish);
	signal(SIGINT, finish);
    signal(SIGSEGV, crash_segv);
	gm->getQueue()->loopToFinish();

	LOG("main") << "Dungeon ends. Bye!" << LOGF;
}

/*
 *	Confirms dbRestart and sets the flag for GM if confirmed
 */
void dbRestart() {
	string answer;
	LOG("main") << "Database cleanup requested. Are you sure? [y/N]" << LOGF;

	getline(cin, answer);
	if (answer != "y") {
		LOG("main") << "Cleanup canceled." << LOGF;
	} else {
        initWorld = true;
	}
}

int main(int argc, char** argv) {
    Logger::initialize();
    
	/*
	 *	Process command line parameters, if any found
	 */
	for (int a = 1; a < argc; a++) {
		if (strcmp(argv[a], "cleanDB") == 0) {
			dbRestart();
        } else if (strcmp(argv[a], "verbose") == 0) {
            LOG("main") << "User has requested verbose logging on stdout by launch argument." << LOGF;
            Logger::getInstance().setMinSeverity(cout, Logger::Severity::Verbose);
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

