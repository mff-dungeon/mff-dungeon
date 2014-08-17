#ifndef ACTIONLIST_HPP
#define	ACTIONLIST_HPP

#include "common.hpp"
#include "Action.hpp"
#include "Actions/MultiTargetAction.hpp"

namespace Dungeon {
    typedef std::multimap<string, Action*> actionMap;
        
    class ActionList : public actionMap {
    public:
        ActionList();
        ActionList(const ActionList& orig);
        virtual ~ActionList();
        
        Action* addAction(Action* action);
        Action* addAction(MultiTargetAction* action);
        void clear();
    };
}

#endif

