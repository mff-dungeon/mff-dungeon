#include "JabberDriver.hpp"

using namespace gloox;

namespace Dungeon {
    JabberDriver::JabberDriver(ActionQueue* queue, Alive* figure, string jabberUsername, string jabberPassword) : TextDriver(queue), connected(false), figure(figure) {
        // initialize Jabber client with credentials
        JID jid(jabberUsername);
        client = new Client(jid, jabberPassword);
    }
    
    JabberDriver::~JabberDriver() {
        if (connected) {
            this->stop();
        }
        
        delete client;
    }
    
    void JabberDriver::worker() {
        cout << "[ JD ] Worker started.\n";
        
        // register event handlers
        client->registerMessageHandler(this);
        client->registerConnectionListener(this);
        
        // connect and block this thread until the connection is closed
        client->connect(true);
        
        cout << "[ JD ] Worker ended." << endl;
        this->queue->stop();
    }
    
    void JabberDriver::stop() {
        // disconnect client
        client->setPresence(Presence::Unavailable, presencePriority());
        client->disconnect();
    }
    
    void JabberDriver::run() {
        // create thread with worker()
        this->workerThread = new thread(&JabberDriver::worker, this);
    }
    
    void JabberDriver::handleMessage(const Message& message, MessageSession* session) {
        Message::MessageType type = message.subtype();
        
        if (type == Message::Chat) {
            // find out who sent what
            string contents = message.body();
            string sender = message.from().bare();
            
            // load corresponding session and log message arrival
            Alive* figure = this->findFigure(sender);
            cout << "[ JD ] User '" << sender << "' sent a message: '" << contents << "'" << endl;
            
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
            
            cout << "[ JD ] Received unsupported message type: '" << typeName << "', sender: '" << sender << "', body: '" << contents << "'" << endl;
        }
    }
    
    void JabberDriver::onConnect() {
        cout << "[ JD ] Connection estabilished." << endl;
        client->setPresence(Presence::Available, presencePriority());
        connected = true;
    }
    
    void JabberDriver::onDisconnect(ConnectionError e) {
        cout << "[ JD ] Connection closed." << e << endl;
        connected = false;
    }
    
    bool JabberDriver::onTLSConnect(const CertInfo& info) {
        cout << "[ JD ] TLS connected." << endl;
        return true;
    }
    
    Alive* JabberDriver::findFigure(string jabberUsername) {
        return figure; // TODO: poll figure from sessions list or spawn a new one
    }
}