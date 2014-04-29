#ifndef XMPPLISTENER_HPP
#define	XMPPLISTENER_HPP

#include <thread>
#include "common.hpp"
#include <boost/bind.hpp>
#include "Swiften/Swiften.h"

namespace Dungeon {

    using namespace Swift;
    using namespace boost;

    class XmppListener {
    public:
        XmppListener();
        virtual ~XmppListener();

        void run();

        void handleConnected();
        void handleDisconnected();
        void handleMessageReceived(Message::ref message);
        void handlePresenceReceived(Presence::ref presence);

    private:
        SimpleEventLoop *eventLoop;
        BoostNetworkFactories *networkFactories;
        std::thread *workerThread;
        Client* client;
        //ClientXMLTracer* tracer;
    };
}

#endif	/* XMPPLISTENER_HPP */

