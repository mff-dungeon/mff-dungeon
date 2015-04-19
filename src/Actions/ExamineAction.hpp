#ifndef EXAMINEACTION_HPP
#define	EXAMINEACTION_HPP

#include "../common.hpp"
#include "../Actions/MultiTargetAction.hpp"

namespace Dungeon
{
    class ExamineEction : public MultiTargetAction {
    public:
            ExamineEction() : MultiTargetAction("examine") { }

            virtual bool match(const string& command, ActionDescriptor* ad);
            virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);
            virtual void explain(ActionDescriptor* ad);
    };
}

#endif

