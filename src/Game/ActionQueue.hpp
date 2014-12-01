#ifndef ACTIONQUEUE_HPP
#define	ACTIONQUEUE_HPP

#include "../common.hpp"
#include "../Game/ActionDescriptor.hpp"
#include "../Game/GameManager.hpp"
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace Dungeon {
    
    class Driver;
    
    /* 
     * Action Queue should receive actions from 
     * threads and commit them sequentially.
     * 
     * Every world-changing code must be run in thread
     * where this queue runs. The world is not and never will
     * be thread safe.
     */
    class ActionQueue {
    public:
        ActionQueue(GameManager* gm);
        
        /**
         * Adds action to the queue. AD will be deleted after processing.
         * @param ad
         */
        void enqueue(ActionDescriptor* ad);
        
        /**
         * Run until stopped (by some action or terminating all drivers).
         */
        void loopToFinish();
        
        /**
         * Tells the queue to stop after processing current event. Thread safe.
         */
        void stop();
                
        typedef queue<ActionDescriptor *> qType;
        
    private:
        /**
         * Process one action (and wait for it if necessary). 
         */
        void process();
        
        qType actions;
        GameManager* gm;
        volatile bool running = true;
        
        mutex q_mutex;
        condition_variable q_condvar;

        typedef lock_guard<std::mutex> lock;
        typedef unique_lock<std::mutex> ulock;
    };
}

#endif	/* ACTIONQUEUE_HPP */

