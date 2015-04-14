#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
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
#include "Utils/Config.hpp"

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
ofstream debugFile;

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
        if (signal == SIGINT) LOG << "Caught another SIGINT, already in shutdown mode." << LOGF;
        else if (signal == SIGTERM) LOG << "Caught another SIGTERM, already in shutdown mode." << LOGF;
        
        LOGS(Fatal) << "Resolving potentially uncontrollable hang by voluntary suicide (SIGABRT)..." << LOGF;
        abort();
        return;
    }
    
	LOGH("Finish");
	if (signal == SIGINT) LOGS(Warning) << "Caught SIGINT, terminating..." << LOGF;
	else if (signal == SIGTERM) LOGS(Warning) << "Caught SIGTERM, terminating..." << LOGF;
    
    finishing = true;

	jabber->stop();
	gm->shutdown();
	if(debugFile.is_open()) debugFile.close();
}

/*
 *  Handles segfault.
 */
void crash_segv(int signal) {
    LOGS(Fatal) << "Caught SIGSEGV (" << signal << "), crashing... " << getStacktace() << LOGF;
	throw GameException("Segmentation fault.");
}

/*
 *	Starts the dungeon and waits until it finishes. Registers signals so
 *		the queue can be stopped.
 */
void start() {
	LOGH("Start");
	LOG << "This is Jabber Dungeon starting." << LOGF;
	gm = new GameManager(initWorld);

	//dungeon@eideo.cz, somemagicshit - release user/passwd
	jabber = new JabberDriver(gm, Config::JabberName(), Config::JabberPasswd());
	jabber->useXHtmlIM = Config::FeatureXHtmlIM();
	jabber->run();
	LOG << "Drivers started, starting queue." << LOGF;

	signal(SIGTERM, finish);
	signal(SIGINT, finish);
    signal(SIGSEGV, crash_segv);
	gm->getQueue()->loopToFinish();

	LOG << "Dungeon ends. Bye!" << LOGF;
}

/*
 * Used by command line argument -h. Prints a short help and argument information.
 */
void printHelp() {
	cout << "This is the Jabber Dungeon. If you don't know what it is and you are trying to start it anyway, "
			<< "please read the documentation first. " << endl
			<< "Usable arguments: " << endl 
			<< "\t-h|--help\t\tPrints this help" << endl 
			<< "\t-f|--config [file]\tSpecifies the path to the config file" << endl
			<< "\t-v|--verbose\t\tThe game will print detailed information on standard output" << endl
			<< "\t-d|--debug\t\tTurns on debug logging" << endl
			<< "\t-p|--printDebug\t\tPrints debug information on the console. Mutually exclusive with -v" << endl
			<< "\t-c|--cleanDB\t\tThe game world will be reset to default state" << endl;
}

/*
 *	Confirms dbRestart and sets the flag for GM if confirmed
 */
void dbRestart() {
	string answer;
	LOG << "Database cleanup requested. Are you sure? [y/N]" << LOGF;

	getline(cin, answer);
	if (answer != "y") {
		LOG << "Cleanup canceled." << LOGF;
	} else {
        initWorld = true;
	}
}

/*
 * Processes command line arguments. 
 * If an invalid argument is found, this method returns false so we may end the program.
 */
bool parseArguments(vector<string>& args) {
	for(auto it = args.begin() + 1; it != args.end(); it++) {
		if (*it == "help" || *it == "--help" || *it == "-h") {
			printHelp();
			return false;
		} else if(*it == "cleanDB" || *it == "--cleanDB" || *it == "-c") {
			dbRestart();
		} else if (*it == "--verbose" || *it == "verbose" || *it == "-v") {
			LOG << "Requested verbose logging on stdout." << LOGF;
			Logger::getInstance().setMinSeverity(cout, Logger::Severity::Verbose);
		} else if (*it == "--debug" || *it == "debug" || *it == "-d") {
			LOG << "Requested debugging information, debug file was opened." << LOGF;
			debugFile.open(Utils::currentTime("%Y-%m-%d") + "_debug.log", fstream::out | fstream::app | fstream::ate);
			Logger::getInstance().linkStream(debugFile, Logger::Severity::Debug);
		} else if (*it == "--printDebug" || *it == "printDebug" || *it == "-p") {
			LOG << "Requested logging debugging information on stdout." << LOGF;
			Logger::getInstance().setMinSeverity(cout, Logger::Severity::Debug);
		} else if (*it == "--config" || *it == "config" || *it == "-f") {
			if(Config::isInitialized()) {
				LOGS(Fatal) << "Config is already initialized." << LOGF;
				return false;
			}
			if(it+1 == args.end()) {
				LOGS(Fatal) << "Specified config argument has no value." << LOGF;
				return false;
			}
			string& fname = *(it+1);
			if(fname.find("-") == 0) {
				LOGS(Fatal) << "Specified config file name starts with a dash. "
						<< "It is possible that you have forgot to put a filename after the config argument. Please use a filename without a dash." << LOGF;
				return false;
			}
			try {
				Config::initialize(fname);
			}
			catch (ConfigParser::InvalidFileException& e) {
				LOGS(Fatal) << "Configuration loading error: " << e.what() << LOGF;
				return false;
			}
			catch (ConfigParser::InvalidFieldException& e) {
				LOGS(Fatal) << "Configuration loading error: Field " << e.fieldName << " - " << e.what() << LOGF;
				return false;
			}
			it++;
		} else {
			LOGS(Fatal) << "Unknown argument: " << *it << LOGF;
			cout << "Unknown argument: " << *it << ". Use -h or --help argument to get a list of accepted arguments." << endl;
			return false;
		}
	}
	return true;
}

int main(int argc, char** argv) {
	#ifndef COMPATIBLE
	cout << "Warning: Our code may not be compatible with your compiler. "
		<< "We are not responsible for any unexpected bugs that may occur to you. "
		<< "For the best results, use gcc version 4.9 and newer, or clang version 3.2 or newer." << endl;
	#endif
	
    Logger::initialize();
    vector<string> args(argv, argv+argc);
	if(!parseArguments(args)) {
		return 1;
	}
	
	/*
	 *  No config is initialized, load the default one
	 */
	if(!Config::isInitialized()) {
		try {
			Config::initialize("config");
		}
		catch (ConfigParser::InvalidFileException& e) {
			LOGS(Fatal) << "Configuration loading error: " << e.what() << LOGF;
			LOGS(Fatal) << "There must be either a configuration file in the current folder called \"config\", "
					<< "or you must specify a path to a valid configuration file using command line arguments." << LOGF;
			return 2;
		}
		catch (ConfigParser::InvalidFieldException& e) {
			LOGS(Fatal) << "Configuration loading error: Field " << e.fieldName << " - " << e.what() << LOGF;
			return 2;
		}
	}
	
	/*
	 *	Regular dungeon start 
	 */
	LOG << "Starting dungeon..." << LOGF;
	start();
	if(debugFile.is_open()) debugFile.close();
	return 0;
}

