#include "common.hpp"
#include "gameManager.cpp"
#include <iostream>

using namespace Dungeon;

int main(int argc, char** argv) {
	std::cout << "This is Jabber Dungeon starting." << std::endl;
	GameManager gm();
	
	/*
	ActionQueue queue (gm);
	XmppListener xmpp (queue);
	ConsoleDriver console (queue, "admin@jabberdung");
    
	xmpp.connect();
	xmpp.startThread();
	queue.startLoop();
	console.run();
	
	queue.finish();
	xmpp.stopThread();
	xmpp.disconnect();
	*/
	std::cout << "Dungeon ends. Bye!" << std::endl;
}

