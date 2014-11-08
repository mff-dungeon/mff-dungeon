#include <iostream>
#include <sstream>
#include <signal.h>		
#include <execinfo.h>
#include <unistd.h>
#include "common.hpp"
#include "Persistence/DatabaseHandler.hpp"
#include "Drivers/JabberDriver.hpp"
#include "Drivers/JabberDriver.hpp"
#include "Game/GameManager.hpp"
#include "Game/ActionQueue.hpp"
#include "dynamic.hpp"

using namespace Dungeon;

/*
 *	Global variables - global in order to be accessable by all functions 
 *		(mainly because of aqueue by finish() )  
 */
GameManager *gm;
Alive* admin;
JabberDriver* jabber;
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
	throw GameException("Segmentation fault.");
}

/*
 *	Starts the dungeon and waits until it finishes. Registers signals so
 *		the queue can be stopped.
 */
void start() {
	LOGH("Start");
	LOG("main") << "This is Jabber Dungeon starting." << LOGF;
	gm = new GameManager(initWorld);

	//jabber = new JabberDriver(gm, "dungeon@eideo.cz", "somemagicshit");
	jabber = new JabberDriver(gm, "dungeon-test@eideo.cz", "losethegame");
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
	#ifndef COMPATIBLE
	cout << "Warning: Our code may not be compatible with your compiler. "
		<< "We are not responsible for any unexpected bugs that may occur to you. "
		<< "For the best results, use gcc version 4.9 and newer, or clang version 3.2 or newer." << endl;
	#endif
	
    Logger::initialize();
    
	/*
	 *	Process command line parameters, if any found
	 */
	for (int a = 1; a < argc; a++) {
		string arg = string(argv[a]);
		if (arg == "cleanDB" || arg == "--cleanDB") {
			dbRestart();
        } else if (arg == "--verbose" || arg == "verbose" || arg == "-v") {
            LOG("main") << "User has requested verbose logging on stdout by launch argument." << LOGF;
            Logger::getInstance().setMinSeverity(cout, Logger::Severity::Verbose);
		} else
			LOG("main") << "Unknown argument: " << argv[a] << LOGF;
	}
	/*
	 *	Regular dungeon start 
	 */
	LOG("main") << "Starting dungeon..." << LOGF;
	start();
	return 0;
}

