#include "common.hpp"
#include "dynamic.hpp"
#include "GameManager.hpp"
#include "ActionQueue.hpp"
#include "ConsoleDriver.hpp"
#include "XmppListener.hpp"
#include <iostream>

using namespace Dungeon;

int main(int argc, char** argv) {
	std::cout << "This is Jabber Dungeon starting." << std::endl;
	GameManager* gm = new GameManager();
	ActionQueue* queue = new ActionQueue (gm);
	
	Alive* admin = new Alive("human/admin@jabberdung");
    
	ConsoleDriver* console = new ConsoleDriver(queue, admin);
	console->run();
        
	XmppListener* xmpp = new XmppListener ();
	xmpp->run();
	 
	
	queue->loopToFinish();
	
	/*
	xmpp.stopThread();
	xmpp.disconnect();
	*/
	std::cout << "Dungeon ends. Bye!" << std::endl;
}

