#ifndef EXAMINEACTION_HPP
#define	EXAMINEACTION_HPP

#include "../common.hpp"
#include "../Actions/MultiTargetAction.hpp"

namespace Dungeon
{
    class ExamineEction : public MultiTargetAction {
    public:
            ExamineEction() : MultiTargetAction("examine") {
                messages.unspecified = "Do you want to look closer on %?";
                messages.dontCare = "If you don't care, I'll just examine %.";
                messages.uncertain = "Sorry, did you want to examine %?";
                messages.noCandidate = "Sorry, I don't understand what to examine. Please try to explain it better.";
            }

            virtual bool match(const string& command, ActionDescriptor* ad);
            virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);
            virtual void explain(ActionDescriptor* ad);
    };
}

#endif

