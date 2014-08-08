#include "JabberDriver.hpp"

using namespace gloox;

namespace Dungeon {
    JabberDriver::JabberDriver(ActionQueue* queue, Alive* figure, string jabberUsername, string jabberPassword) : TextDriver(queue), connected(false), figure(figure) {
        // initialize Jabber client with credentials
        JID jid(jabberUsername);
        client = new Client(jid, jabberPassword);
        
        client->disco()->setVersion("Dungeon", "1.0");
        client->disco()->setIdentity("client", "bot", "Dungeon");
    }
    
    JabberDriver::~JabberDriver() {
        if (connected) {
            this->stop();
        }
        
        delete client;
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
        this->queue->stop();
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
		// get the Alive, message and send a response
        Alive *figure = descriptor->getAlive();
        JID jid = this->findJID(figure);
        
        if (!jid) {
            return;
        }
        
        Message msg(Message::Chat, jid, descriptor->message);
        client->send(msg);
	}
	
    void JabberDriver::handleMessage(const Message& message, MessageSession* session) {
        Message::MessageType type = message.subtype();
        
        if (type == Message::Chat) {
            // find out who sent what
            string contents = message.body();
            string sender = message.from().bare();
            
            if (contents == "") {
                return;
            }
            
            // load corresponding session and log message arrival
            Alive* figure = this->findFigure(message.from());
            LOG("JabberDriver") << "User '" << sender << "' sent a message: '" << contents << "'" << LOGF;
            
            // produce a response
            string response = this->process(contents, figure);
            
            // send the response back to the client
            Message msg(Message::Chat, message.from(), response);
            client->send(msg);
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
                    
                case Message::Normal:
                    typeName = "normal";
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
    
    Alive* JabberDriver::findFigure(JID jid) {
        JabberSession *foundSession = nullptr;
        for (auto &session : sessions) {
            if (session.getJID() == jid) {
                foundSession = &session;
                break;
            }
        }
        
        if (!foundSession) {            
            JabberSession newSession(jid, figure);
            sessions.push_back(newSession);
            
            foundSession = &newSession;
        }
        
        return foundSession->getFigure();
    }
    
    JID JabberDriver::findJID(Alive *figure) {
        JabberSession *foundSession = nullptr;
        for (auto &session : sessions) {
            if (session.getFigure() == figure) {
                foundSession = &session;
                break;
            }
        }
        
        return foundSession ? foundSession->getJID() : JID();
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
    
    JabberDriver::JabberSession::JabberSession(JID jid, Alive* figure) : m_jid(jid), m_figure(figure) {
        
    }
    
    JabberDriver::JabberSession::~JabberSession() {
        
    }
    
    bool JabberDriver::JabberSession::isExpired() {
        return false;
    }
    
    void JabberDriver::JabberSession::renew() {
        
    }
    
    Alive* JabberDriver::JabberSession::getFigure() {
        
    }
    
    JID JabberDriver::JabberSession::getJID() {
        
    }
}
