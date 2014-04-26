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
        ActionDescriptor *enqueue(Action *action);
        void process();
        void loopToFinish();
        
        typedef queue<ActionDescriptor *> qType;
    private:
        qType actions;
        GameManager* gm;
        bool running = true;
        
        mutex q_mutex;
        condition_variable q_condvar;

        typedef lock_guard<std::mutex> lock;
        typedef unique_lock<std::mutex> ulock;
    };
}

#endif	/* ACTIONQUEUE_HPP */

