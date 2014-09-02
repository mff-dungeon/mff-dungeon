/* 
 * Should test all the classes. Should.
 */

#include "../src/common.hpp"
#include "testutils.cpp"
#include "../src/dynamic.hpp"
#include "../src/FuzzyStringMatcher.hpp"
#include "../src/RandomString.hpp"

void testFSMatcher() {
    testing("FuzzyMatcher");
    FuzzyStringMatcher<int> matcher;
    assertEqual(matcher.getDifference("Sample string.", "Sapmle string."), matcher.transCost, "Transpose");
    assertEqual(matcher.getDifference("Sample string.", "Sample sfring."), matcher.substCost, "Substitute");
    assertEqual(matcher.getDifference("Sample string.", "Sampled string."), matcher.insertCost, "Insert");
    assertEqual(matcher.getDifference("Sample string.", "Smple string."), matcher.deleteCost, "Delete");
    assertEqual(matcher.getDifference("Sample string.", "Sample string."), 0, "Identical strings");
    matcher.add("test", 1);
    assert(matcher.find("test") == 1, "Single object");

    matcher.clear();
    matcher.add("death potion", 1);
    matcher.add("health potion", 2);
    assert(matcher.find("the health one") == 2, "Potion selection 1");
    assert(matcher.find("health") == 2, "Potion selection 2");

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
    assertEqual(matcher.find("nooooo!"), 0, "True/false 1");
    assertEqual(matcher.find("y"), 1, "True/false 2");
    assertEqual(matcher.find("n"), 0, "True/false 3");

    matcher.clear();
    matcher.add("Red potion", 1);
    matcher.add("Blue potion", 2);
    matcher.add("Green potion", 2);
    assert(matcher.find("blue") == 2, "Potion selection 3");
}

void testInstanceOf() {
	Human* h = new Human();
	
	testing("InstanceOf");
	assertEqual(h->instanceOf(Human), true, "Same class");
	assertEqual(h->instanceOf(Alive), true, "Inherited");
	assertEqual(h->instanceOf(IObject), true, "Base class");
	assertEqual(h->instanceOf(Backpack), false, "Different class");
}

void testRandomString() {
	testing("Random string");
	RandomString rand;
	rand << "Test message 1" << endr
			<< "Test message 2" << endr;
	
	assertEqual<string>(((string) rand).substr(0, 4), "Test", "String operator");
	
	string r = RandomString::get()
			<< "Test message 1" << endr
			<< "Test message 2" << endr;
	
	assertEqual<string>(r.substr(0, 4), "Test", "Static use operator");
}
int main(int argc, char** argv) {
    testStart();
	
    testFSMatcher();
	testInstanceOf();
	testRandomString();
    // Add your own test method
    
    return testFinish();
}