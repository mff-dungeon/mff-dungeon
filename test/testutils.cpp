
#include "../src/Logger.hpp"

using namespace Dungeon;
static int failedTests = 0;
static string ns = "";

void assert(bool cond, string msg) {
    if (!cond) {
        LOGS("Tester", Error) << "[E] " << ns << ":" << msg << LOGF;
        failedTests++;
    }
}

template<typename T>
void assertEqual(T a, T b, string msg) {
    if (a != b) {
        LOGS("Tester", Error) << "[E] " << ns << ":" << msg << " expected (" << b << ") got (" << a << ")" << LOGF;
        failedTests++;
    }
}

void testing(string _namespace) {
    ns = _namespace;
}

void testStart() {
    Logger::initialize();    
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
