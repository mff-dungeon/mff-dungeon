#include "MultiTargetAction.hpp"
#include "../ObjectPointer.hpp"
#include "../ObjectGroup.hpp"
#include "../FuzzyStringMatcher.hpp"
#include "../ActionDescriptor.hpp"

namespace Dungeon {

	MultiTargetAction::~MultiTargetAction() {
	}

	MultiTargetAction::MultiTargetAction(string type) : Action(type) {}

	void MultiTargetAction::addTarget(ObjectPointer op) {
		if (targets.find(op.getId()) == targets.end())
			targets[op.getId()] = op;
	}
	
	void MultiTargetAction::merge(MultiTargetAction* second) {
		for (auto& pair : second->getTargets()) {
			addTarget(pair.second);
		}
		delete second;
	}
	
	const ObjectMap& MultiTargetAction::getTargets() const {
		return targets;
	}
	
        void MultiTargetAction::commit(ActionDescriptor* ad) {
            commitOnBestTarget(ad, ad->in_msg);
        }
        
        void MultiTargetAction::commitOnBestTarget(ActionDescriptor* ad, string str) {
            ObjectGroup group (targets);
            ObjectPointer target;
            try {
                target = group.match(str);
                IDescriptable* obj = target.safeCast<IDescriptable>();
                LOGS("MTA", Verbose) << "Selected " << obj->getLongName() << LOGF;
                commitOnTarget(ad, target);
            } catch (const StringMatcher::Uncertain& e) {
                *ad << "I'm sorry, did you say \"" << "\"?";
                ad->waitForReply([this] (ActionDescriptor* ad, string reply) {
                    bool repl = StringMatcher::matchTrueFalse(reply);
                    if (repl) {
                        // commitOnTarget(ad, target);
                    } else {
                        *ad << "Sorry, i'm sort of dumb last few days. Please say it again.";
                    }
                });
            } catch (const StringMatcher::NoCandidate& e) {
                *ad << "No such thing found. Really.";
            }
        }
}
