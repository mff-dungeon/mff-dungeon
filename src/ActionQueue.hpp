/* 
 * Action Queue should receive actions from 
 * threads and commit them sequentially
 */

#ifndef ACTIONQUEUE_HPP
#define	ACTIONQUEUE_HPP

#include "Action.hpp"
#include "ActionDescriptor.hpp"
#include "GameManager.hpp"
#include <queue>

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
    };
}

#endif	/* ACTIONQUEUE_HPP */

