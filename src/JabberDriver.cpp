#include <netdb.h>

#include "JabberDriver.hpp"
#include "ActionDescriptor.hpp"

using namespace gloox;

namespace Dungeon {
    JabberDriver::JabberDriver(GameManager* gm, string jabberUsername, string jabberPassword) : TextDriver(gm->getQueue()), connected(false), gm(gm) {
        // initialize Jabber client with credentials
        JID jid(jabberUsername);
        client = new Client(jid, jabberPassword);
        
        client->disco()->setVersion("Dungeon", "1.0");
        client->disco()->setIdentity("client", "bot", "Dungeon");
        
        userFile.open("jabber_sessions", fstream::in | fstream::out | fstream::app);
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
        LOG("JabberDriver") << "Worker started." << LOGF;
        
        // register event handlers
        client->registerMessageHandler(this);
        client->registerPresenceHandler(this);
        client->registerSubscriptionHandler(this);
        client->registerConnectionListener(this);
        
        // connect and block this thread until the connection is closed
        client->connect(true);
        
        LOG("JabberDriver") << "Worker ended." << LOGF;
    }
    
    void JabberDriver::stop() {
        // disconnect client
        client->setPresence(Presence::Unavailable, presencePriority(), "I will be back.");
        client->disconnect();
    }
    
    void JabberDriver::run() {
        // create thread with worker()
        this->workerThread = new thread(&JabberDriver::worker, this);
    }
    
	void JabberDriver::processDescriptor(ActionDescriptor* descriptor) {
		TextActionDescriptor* ad = (TextActionDescriptor*) descriptor;
		objId figureId = this->findFigureId(ad->from);
		Alive* figure = (Alive*) gm->getObject(figureId);
		ad->assigned(figure);
		
		this->process(ad);
		
        Message msg(Message::Chat, ad->from, ad->getReply());
        client->send(msg);
	}
	
    void JabberDriver::handleMessage(const Message& message, MessageSession* session) {
        Message::MessageType type = message.subtype();
        
        if (type == Message::Chat || type == Message::Normal) {
            // find out who sent what
            string contents = message.body();
            string sender = message.from().bare();
            
            if (contents == "") {
                return;
            }
			
            LOG("JabberDriver") << "User '" << sender << "' sent a message: '" << contents << "'" << LOGF;
            
            TextActionDescriptor* ad = new TextActionDescriptor(this);
			ad->from.assign(sender);
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
            
            LOG("JabberDriver") << "Received unsupported message type: '" << typeName << "', sender: '" << sender << "', body: '" << contents << "'" << LOGF;
        }
    }
    
    void JabberDriver::onConnect() {
        LOG("JabberDriver") << "Connection estabilished." << LOGF;
        client->setPresence(Presence::Available, presencePriority(), "The adventure awaits!");
        connected = true;
    }
    
    void JabberDriver::onDisconnect(ConnectionError e) {
        LOG("JabberDriver") << "Connection closed. Error: " << e << LOGF;
        connected = false;
    }
    
    bool JabberDriver::onTLSConnect(const CertInfo& info) {
        LOG("JabberDriver") << "TLS connected." << LOGF;
        return true;
    }
    
    objId JabberDriver::findFigureId(JID jid) {
		return "human/" + jid.bare();
    }
    
    JID JabberDriver::findJID(objId figureId) {
        JID jid;
        string buffer;
        bool found = false;
        
        LOG("JabberDriver") << "Looking for objId: '" << figureId << "'." << LOGF;
        
        userFile.seekg(0, ios_base::beg);
        while (getline(userFile, buffer, userFileSeparator())) {
            string sessionJid = buffer;
            string sessionObjId;
            
            getline(userFile, sessionObjId, userFileSeparator());
            getline(userFile, buffer);
            
            if (sessionObjId == figureId) {
                found = true;
                
                jid = JID(sessionJid);
                LOG("JabberDriver") << "Found corresponding JID: '" << sessionJid << "'." << LOGF;
                break;
            }
        }
        userFile.clear();
        
        if (!found) {
            LOG("JabberDriver") << "Not found, discarding..." << LOGF;
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
        
        LOG("JabberDriver") << "Received presence: '" << typeName << "', sender: '" << sender << "', status: '" << status << "'" << LOGF;
    }
    
    void JabberDriver::handleSubscription(const Subscription &subscription) {
        string typeName;
        string sender = subscription.from().bare();
        string status = subscription.status();
        
        switch (subscription.subtype()) {
            case Subscription::Subscribe:
                typeName = "subscription request";
                
                // acknowledge request and send similar request back
                this->client->rosterManager()->ackSubscriptionRequest(subscription.from(), true);
                this->client->rosterManager()->subscribe(subscription.from(), EmptyString, StringList(), "The Dungeon wants you!");
                break;
                
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
        
        LOG("JabberDriver") << "Received subcription packet: '" << typeName << "', sender '" << sender << "', status: '" << status << "'" << LOGF;
    }
}
