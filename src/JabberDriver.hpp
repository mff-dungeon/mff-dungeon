#ifndef JABBERDRIVER_HPP
#define	JABBERDRIVER_HPP

#include <thread>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <string>
#include <gloox/client.h>
#include <gloox/message.h>
#include <gloox/messagehandler.h>
#include <gloox/presencehandler.h>
#include <gloox/connectionlistener.h>
#include <gloox/subscriptionhandler.h>
#include <gloox/disco.h>
#include <gloox/rostermanager.h>
#include "common.hpp"
#include "TextDriver.hpp"
#include "Objects/Alive.hpp"

namespace Dungeon {
    
    /** Handles all user communication over XMPP (mainly Jabber).
     */
    class JabberDriver : public TextDriver, public gloox::MessageHandler, public gloox::ConnectionListener, public gloox::PresenceHandler, public gloox::SubscriptionHandler {
    public:
        /** Constructs new instance of JabberDriver
         @param queue   Queue which accepts all actions generated by the driver.
         @param jabberUsername  Username credential for Jabber connection.
         @param jabberPassword  Password credential for Jabber connection.
         */
        JabberDriver(GameManager* gm, string jabberUsername, string jabberPassword);
        virtual ~JabberDriver();
        
        /** Connects the driver to the Jabber server and starts listening for new messages.
         */
        void run();
        
        /** Stops listening and disconnects the driver from the Jabber server
         */
        void stop();
		
        virtual void processDescriptor(ActionDescriptor* descriptor);
        
        /* gloox::MessageHandler interface implementation */
        void handleMessage(const gloox::Message& message, gloox::MessageSession* session = 0);
        
        /* gloox::ConnectionListener interface implementation */
        void onConnect();
        void onDisconnect(gloox::ConnectionError e);
        bool onTLSConnect(const gloox::CertInfo& info);
        
        /* gloox::PresenceHandler interface implementation */
        void handlePresence(const gloox::Presence &presence);
        
        /* gloox::SubscriptionHandler interface implementation */
        void handleSubscription(const gloox::Subscription &subscription);
        
        bool connected;
        
    private:
        void worker();
        GameManager* gm;
        thread *workerThread;
        gloox::Client* client;
        
        objId findFigureId(gloox::JID jid);
        gloox::JID findJID(objId figureId);
        
        static int presencePriority() { return 0; }
        static char userFileSeparator() { return ':'; }
        fstream userFile;
    };
    
}

#endif	/* JABBERDRIVER_HPP */

