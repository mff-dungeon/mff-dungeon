#include "XmppListener.hpp"

namespace Dungeon {

	XmppListener::XmppListener() {
		eventLoop = new SimpleEventLoop();
		networkFactories = new BoostNetworkFactories(eventLoop);
	}

	XmppListener::~XmppListener() {
	}

	void XmppListener::run() {
		client = new Client("dungeon@eideo.cz", "somemagicshit", networkFactories);
		client->setAlwaysTrustCertificates();
		client->onConnected.connect(bind(&XmppListener::handleConnected, this));
		client->onDisconnected.connect(bind(&XmppListener::handleDisconnected, this));
		client->onMessageReceived.connect(bind(&XmppListener::handleMessageReceived, this, _1));
		client->onPresenceReceived.connect(bind(&XmppListener::handlePresenceReceived, this, _1));
		// tracer = new ClientXMLTracer(client); // Debugging XMPP
		client->connect();
		
		cout << "[ XM ] Starting thread" << endl;
        this->workerThread = new std::thread(&SimpleEventLoop::run, eventLoop);
	}

	void XmppListener::handleConnected() {
		cout << "[ XMPP ] Connected" << endl;
		client->sendPresence(Presence::create("Up and running!"));
	}
	
	void XmppListener::handleDisconnected() {
		cout << "[ XMPP ] Disconnected: " << endl;
		client->connect();
	}

	void XmppListener::handleMessageReceived(Message::ref message) {
		if (message->getBody() == "") {
			cout << "[ XMPP ] " << message->getFrom() << " is composing..." <<  endl;
			return;
		}
		cout << "[ XMPP ] Message from " << message->getFrom() << endl;
		message->setTo(message->getFrom());
		message->setFrom("dungeon@eideo.cz");
		message->setBody("Sorry, I can't understand your request. Try again later!");
		client->sendMessage(message);
	}

	void XmppListener::handlePresenceReceived(Presence::ref presence) {
		if (presence->getType() == Presence::Subscribe) {
			Presence::ref response = Presence::create();
			response->setTo(presence->getFrom());
			response->setType(Presence::Subscribed);
			client->sendPresence(response);
		}
	}
}