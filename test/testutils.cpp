
#include "../src/Utils/Logger.hpp"

using namespace Dungeon;
static int failedTests = 0;
static string ns = "";

void assert(bool cond, string msg) {
    if (!cond) {
        LOGS("Tester", Error) << "[ F ] " << ns << ":" << msg << LOGF;
        failedTests++;
    } else {
        LOGS("Tester", Verbose) << "[ . ] " << ns << ":" << msg << LOGF;
	}
}

template<typename T>
void assertEqual(T a, T b, string msg) {
    if (a != b) {
        LOGS("Tester", Error) << "[ F ] " << ns << ":" << msg << " expected (" << b << ") got (" << a << ")" << LOGF;
        failedTests++;
    } else {
        LOGS("Tester", Verbose) << "[ . ] " << ns << ":" << msg << LOGF;
	}
}

void testing(string _namespace) {
    ns = _namespace;
	LOGS("Tester", Info) << "[---] " << ns << LOGF;
}

void testStart() {
    Logger::initialize();    
	//Logger::getInstance().setMinSeverity(cout, Logger::Severity::Verbose);
    LOGH("Tester started");
}

int testFinish() {
    LOGH("Tester finished.");
    
    if (failedTests) {
        LOGS("Tester", Error) << "Some tests failed!" << LOGF;
        return failedTests;
    } 

    LOG("Tester") << "All OK!" << LOGF;
    return 0;
}
