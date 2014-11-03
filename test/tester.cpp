/* 
 * Should test all the classes. Should.
 */

#include "../src/common.hpp"
#include "testutils.cpp"
#include "../src/dynamic.hpp"
#include "../src/FuzzyStringMatcher.hpp"
#include "../src/RandomString.hpp"
#include "../src/Objects/Item.hpp"
#include "../src/Cloner.hpp"

void testFSMatcher() {
    testing("FuzzyMatcher");
    FuzzyStringMatcher<int> matcher;
    assertEqual(matcher.getWordDistance("Sample string.", "Sapmle string."), matcher.transCost, "Transpose");
    assertEqual(matcher.getWordDistance("Sample string.", "Sample sfring."), matcher.substCost, "Substitute");
    assertEqual(matcher.getWordDistance("Sample string.", "Sampled string."), matcher.insertCost, "Insert");
    assertEqual(matcher.getWordDistance("Sample string.", "Smple string."), matcher.deleteCost, "Delete");
    assertEqual(matcher.getWordDistance("Sample string.", "Sample string."), 0, "Identical strings");
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
    matcher.add("y", 1);
    matcher.add("1", 1);
    matcher.add("yeah", 1);
    matcher.add("ok", 1);
    matcher.add("of course", 1);
    matcher.add("no", 0);
    matcher.add("n", 0);
    matcher.add("0", 0);
    matcher.add("false", 0);
    matcher.add("nope", 0);
    matcher.add("not", 0);
    matcher.add("of course not", 0);
    assertEqual(matcher.find("no"), 0, "True/false 1");
    assertEqual(matcher.find("y"), 1, "True/false 2");
    assertEqual(matcher.find("n"), 0, "True/false 3");
	try {
		matcher.find("hippo");
		assert(false, "NoCandidate");
	} catch (StringMatcher::NoCandidate& e) {
		assert(true, "NoCandidate");
	}

    matcher.clear();
    matcher.add("Red potion", 1);
    matcher.add("Blue potion", 2);
    matcher.add("Green potion", 2);
    assert(matcher.find("blue") == 2, "Potion selection 3");
	
	// Bug #9
	matcher.clear();
	try {
		matcher.add("something", 1);
		matcher.add("", 0);
		matcher.find("");
		assert(true, "Bug #9");
	} catch (std::logic_error& e) {
		assert(false, "Bug #9");
	} catch (StringMatcher::NoCandidate& e) {
		assert(true, "Bug #9");
	}
}

void testInstanceOf() {
	Human* h = new Human();
	
	testing("InstanceOf");
	assertEqual(h->instanceOf(Human), true, "Same class");
	assertEqual(h->instanceOf(Alive), true, "Inherited");
	assertEqual(h->instanceOf(Base), true, "Base class");
	assertEqual(h->instanceOf(Inventory), false, "Different class");
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

void testCloner() {
	testing("Cloner");
	GameManager* gm = new GameManager();
	Item* i = new Item("Testing Item");
	gm->insertObject(i);
	i->setName("Test name")->setDescription("Some going to be cloned item");
	
	Cloner cl = Cloner(i);
	ObjectPointer other = cl.getShallowClone();
	Item* j = other.safeCast<Item>();
	assertEqual<string>(i->getName(), j->getName(), "Name is different");
	assertEqual<string>(i->getDescription(), j->getDescription(), "Description is different");
}

int main(int argc, char** argv) {
    testStart();
	
    testFSMatcher();
	testInstanceOf();
	testRandomString();
	testCloner();
    // Add your own test method
    
    return testFinish();
}