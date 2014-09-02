/* 
 * Should test all the classes. Should.
 */

#include "../src/common.hpp"
#include "testutils.cpp"
#include "../src/dynamic.hpp"
#include "../src/FuzzyStringMatcher.hpp"

void testFSMatcher() {
    testing("FuzzyMatcher");
    FuzzyStringMatcher<int> matcher;
    assertEqual(matcher.getDifference("Sample string.", "Sapmle string."), matcher.transCost, "Transpose doesn't work");
    assertEqual(matcher.getDifference("Sample string.", "Sample sfring."), matcher.substCost, "Substitute doesn't work");
    assertEqual(matcher.getDifference("Sample string.", "Sampled string."), matcher.insertCost, "Insert doesn't work");
    assertEqual(matcher.getDifference("Sample string.", "Smple string."), matcher.deleteCost, "Delete doesn't work");
    assertEqual(matcher.getDifference("Sample string.", "Sample string."), 0, "Identical strings does'nt work");
    matcher.add("test", 1);
    assert(matcher.find("test") == 1, "Wrong result");

    matcher.clear();
    matcher.add("death potion", 1);
    matcher.add("health potion", 2);
    assert(matcher.find("the health one") == 2, "Potion selection failed");
    assert(matcher.find("health") == 2, "Potion selection failed");

    matcher.clear();
    matcher.add("true", 1);
    matcher.add("yes", 1);
    matcher.add("yeah", 1);
    matcher.add("ok", 1);
    matcher.add("of course", 1);
    matcher.add("no", 0);
    matcher.add("false", 0);
    matcher.add("nope", 0);
    matcher.add("not", 0);
    matcher.add("of course not", 0);
    assertEqual(matcher.find("nooooo!"), 0, "True/false will fail");
    assertEqual(matcher.find("y"), 1, "True/false will fail");
    assertEqual(matcher.find("n"), 0, "True/false will fail");

    matcher.clear();
    matcher.add("Red potion", 1);
    matcher.add("Blue potion", 2);
    matcher.add("Green potion", 2);
    assert(matcher.find("blue") == 2, "Potion selection failed");
}

void testInstanceOf() {
	Human* h = new Human();
	
	testing("InstanceOf");
	assertEqual(h->instanceOf(Human), true, "Same class does not work");
	assertEqual(h->instanceOf(Alive), true, "Inherited class does not work");
	assertEqual(h->instanceOf(IObject), true, "Base class does not work");
	assertEqual(h->instanceOf(Backpack), false, "Different class does not work");
}

int main(int argc, char** argv) {
    testStart();
    
    testFSMatcher();
    // Add your own test method
    
    return testFinish();
}