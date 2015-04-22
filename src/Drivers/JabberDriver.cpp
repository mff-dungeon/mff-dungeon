#include <netdb.h>
#include <unistd.h>
#include <chrono>

#include "JabberDriver.hpp"
#include "../Game/ActionDescriptor.hpp"
#include "../Objects/Human.hpp"

using namespace gloox;

namespace Dungeon {

	JabberDriver::JabberDriver(GameManager* gm, string jabberUsername, string jabberPassword) : TextDriver(gm->getQueue()), connected(false), gm(gm), shuttingDown(false) {
		// initialize Jabber client with credentials
		JID jid(jabberUsername);
		char hostname[1024];
		client = new Client(jid, jabberPassword);
		gethostname(hostname, 1024);
		client->setResource(hostname);

		client->disco()->setVersion("Dungeon", "1.0");
		client->disco()->setIdentity("client", "bot", "Dungeon");

		userFile.open("jabber_sessions.db", fstream::in | fstream::out | fstream::app);
	}

	JabberDriver::~JabberDriver() {
		if (connected) {
			this->stop();
		}

		delete client;

		userFile.flush();
		userFile.close();
	}

	void JabberDriver::worker() {
		LOG << "Worker started." << LOGF;

		// register event handlers
		client->registerMessageHandler(this);
		client->registerPresenceHandler(this);
		client->registerSubscriptionHandler(this);
		client->registerConnectionListener(this);

		// connect and block this thread until the connection is closed
		client->connect(true);

		LOG << "Worker ended." << LOGF;
	}

	void JabberDriver::stop() {
		shuttingDown = true;

		// disconnect client
		client->setPresence(Presence::Unavailable, presencePriority(), "I will be back.");
		client->disconnect();
	}

	void JabberDriver::run() {
		shuttingDown = false;

		// create thread with worker()
		this->workerThread = new thread(&JabberDriver::worker, this);
	}

	void JabberDriver::processDescriptor(ActionDescriptor* descriptor) {
		TextActionDescriptor* ad = (TextActionDescriptor*) descriptor;
		objId figureId = this->findFigureId(ad->from);
		Human* figure = gm->getObject(figureId)
				.assertType<Human>("I work only with Humans.")
				.unsafeCast<Human>();
		// Really need to do it on every reply. Alive object could have been disposed in meantime...
		ad->assigned(figure);

		// Let user know we are thinking about him right now
		Message stateMsg(Message::Chat, ad->from);
		stateMsg.addExtension(new ChatState(ChatStateComposing));
		client->send(stateMsg);

		// Do the actual thinking
                try {
                    this->process(ad);
                } catch(...) {
                    Message stateMsg2(Message::Chat, ad->from);
                    stateMsg2.addExtension(new ChatState(ChatStatePaused));
                    client->send(stateMsg2);
                    
                    throw;
                }
		

                // Reply
		const Output::Base& message = ad->getReply();
		Message responseMsg(Message::Chat, ad->from, message.plainString());
		responseMsg.addExtension(new ChatState(ChatStateActive));

		if (useXHtmlIM) {
			Tag xhtml ("html");
			xhtml.setXmlns(gloox::XMLNS_XHTML_IM);
			Tag * body = new Tag(&xhtml, "body");
			body->addAttribute(gloox::XMLNS, "http://www.w3.org/1999/xhtml");
			message.xhtml(body);
			LOGS(Debug) << "XHtmlIM response: " << xhtml.xml() << LOGF;
			responseMsg.addExtension(new XHtmlIM(&xhtml));
		}
		ad->clearReply();
		client->send(responseMsg);

		if (ad->isFinished()) {
			dialogs.erase(figureId);
			delete ad;
		}
	}

	void JabberDriver::handleMessage(const Message& message, MessageSession* session) {
		Message::MessageType type = message.subtype();

		if (type == Message::Chat || type == Message::Normal) {
			// find out who sent what
			string contents = message.body();
			string sender = message.from().bare();

			if (contents == "") {
				// ignore empty messages
				return;
			}

			objId figureId = this->findFigureId(message.from());
			if (!this->gm->hasObject(figureId)) {
				this->createNewFigure(message.from());
				return;
			}

			LOGS(Verbose) << "User '" << figureId << "' sent a message: '" << contents << "'" << LOGF;

			TextActionDescriptor* ad;
			if (isInDialog(figureId)) {
                                LOGS(Verbose) << "Responding in dialog mode." << LOGF;
				ad = dialogs[figureId];
			} else {
                                LOGS(Verbose) << "Creating new dialog." << LOGF;
				ad = new TextActionDescriptor(this);
				dialogs[figureId] = ad;
				ad->from.assign(sender);
			}
			ad->in_msg.assign(contents);

			queue->enqueue(ad);
		} else {
			// unsupported message type

			string typeName;
			switch (type) {
				case Message::Error:
					typeName = "error";
					break;

				case Message::Groupchat:
					typeName = "group chat";
					break;

				case Message::Headline:
					typeName = "headline";
					break;

				case Message::Invalid:
					typeName = "invalid";
					break;

				default:
					typeName = "unknown";
					break;
			}

			string contents = message.body();
			string sender = message.from().bare();

			LOGS(Verbose) << "Received unsupported message type: '" << typeName << "', sender: '" << sender << "', body: '" << contents << "'" << LOGF;
		}
	}

	void JabberDriver::onConnect() {
		LOG << "Connection estabilished." << LOGF;
		client->setPresence(Presence::Available, presencePriority(), "The adventure awaits!");
		connected = true;
	}

	void JabberDriver::onDisconnect(ConnectionError e) {
		connected = false;

		if (!shuttingDown) {
			LOGS(Error) << "Connection lost, retrying. Error: " << e << LOGF;

			this_thread::sleep_for(chrono::seconds(10));
			client->connect(true);
		} else {
			LOG << "Connection closed. Error: " << e << LOGF;
		}
	}

	bool JabberDriver::onTLSConnect(const CertInfo& info) {
		LOGS(Verbose) << "TLS connected. [ Valid: " << info.chain << ", Server: '" << info.server << "', Issuer: '" << info.issuer << "' ]" << LOGF;
		return true;
	}

	objId JabberDriver::findFigureId(JID jid) {
		return "human/" + jid.bare();
	}

	JID JabberDriver::findJID(objId figureId) {
		JID jid;
		string buffer;
		bool found = false;

		LOGS(Debug) << "Looking for figure with objId: '" << figureId << "'." << LOGF;

		userFile.seekg(0, ios_base::beg);
		while (getline(userFile, buffer, userFileSeparator())) {
			string sessionJid = buffer;
			string sessionObjId;

			getline(userFile, sessionObjId, userFileSeparator());
			getline(userFile, buffer);

			if (sessionObjId == figureId) {
				found = true;

				jid = JID(sessionJid);
				LOGS(Debug) << "Found corresponding JID: '" << sessionJid << "'." << LOGF;
				break;
			}
		}
		userFile.clear();

		if (!found) {
			LOGS(Debug) << "Not found, discarding..." << LOGF;
		}

		return jid;
	}

	void JabberDriver::handlePresence(const Presence &presence) {
		string sender = presence.from().bare();
		string status = presence.status();
		string typeName;

		switch (presence.subtype()) {
			case Presence::Available:
				typeName = "available";
				break;

			case Presence::Chat:
				typeName = "chat";
				break;

			case Presence::Away:
				typeName = "away";
				break;

			case Presence::DND:
				typeName = "do not disturb";
				break;

			case Presence::XA:
				typeName = "extended away";
				break;

			case Presence::Unavailable:
				typeName = "unavailable";
				break;

			case Presence::Probe:
				typeName = "probe";
				break;

			case Presence::Error:
				typeName = "error";
				break;

			case Presence::Invalid:
				typeName = "invalid";
				break;

			default:
				typeName = "unknown";
				break;
		}

		LOGS(Debug) << "Received presence: '" << typeName << "', sender: '" << sender << "', status: '" << status << "'" << LOGF;
	}

	void JabberDriver::createNewFigure(JID jid) {
		objId figureId = this->findFigureId(jid);
		LOGS(Info) << "Yay! We've got a new player: " << figureId << LOGF;

		Alive* figure = new Human(figureId, jid.username(), jid.bare());
		this->gm->addNewFigure(figure);

		TextActionDescriptor* ad = new TextActionDescriptor(this);
		dialogs[figureId] = ad;
		ad->from.assign(jid.bare());
		ad->matched(this->getCreateAction());

		queue->enqueue(ad);
	}

	void JabberDriver::handleSubscription(const Subscription &subscription) {
		string typeName;
		string sender = subscription.from().bare();
		string status = subscription.status();

		switch (subscription.subtype()) {
			case Subscription::Subscribe:
			{
				typeName = "subscription request";

				// create new user if necessarydialog
				objId figureId = this->findFigureId(subscription.from());
				if (!this->gm->hasObject(figureId)) {
					this->createNewFigure(subscription.from());
				}

				// acknowledge request and send similar request back
				this->client->rosterManager()->ackSubscriptionRequest(subscription.from(), true);
				this->client->rosterManager()->subscribe(subscription.from(), EmptyString, StringList(), "The Dungeon wants you!");
				break;
			}

			case Subscription::Subscribed:
				typeName = "subscribed notification";
				break;

			case Subscription::Unsubscribe:
				typeName = "unsubscription request";

				// acknowledge request and send similar request back
				this->client->rosterManager()->cancel(subscription.from(), "Bye!");
				this->client->rosterManager()->remove(subscription.from());
				break;

			case Subscription::Unsubscribed:
				typeName = "unsubscribed notification";
				break;

			default:
				break;
		}

		LOGS(Debug) << "Received subcription packet: '" << typeName << "', sender '" << sender << "', status: '" << status << "'" << LOGF;
	}
}
