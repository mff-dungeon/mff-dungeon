/* 
 * Action Queue should receive actions from 
 * threads and commit them sequentially
 */

#ifndef ACTIONQUEUE_HPP
#define	ACTIONQUEUE_HPP

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "common.hpp"
#include "Action.hpp"
#include "ActionDescriptor.hpp"
#include "GameManager.hpp"


namespace Dungeon {

    class ActionQueue {
    public:
        ActionQueue(GameManager* gm);
        ActionDescriptor *enqueue(Action *action, Alive* caller);
        void process();
        void loopToFinish();
        void stop();
        
        void registerDriver(Driver *driver);
        void unregisterDriver(Driver *driver);
        
        typedef queue<ActionDescriptor *> qType;
    private:
        qType actions;
        GameManager* gm;
        volatile bool running = true;
        
        mutex q_mutex;
        condition_variable q_condvar;

        typedef lock_guard<std::mutex> lock;
        typedef unique_lock<std::mutex> ulock;
        
        vector<Driver *> drivers;
    };
}

#endif	/* ACTIONQUEUE_HPP */

