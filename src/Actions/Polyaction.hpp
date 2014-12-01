#ifndef POLYACTION_HPP
#define	POLYACTION_HPP

#include "../common.hpp"
#include "MultiTargetAction.hpp"

namespace Dungeon {

	class Polyaction : public MultiTargetAction {
	public:
		Polyaction(string type) : MultiTargetAction(type) { };
		virtual ~Polyaction();

        Polyaction& operator=(const Polyaction& right);
        Polyaction& operator=(Polyaction && right);

		virtual MultiTargetAction* addTarget(ObjectPointer op);
		virtual Polyaction* addTarget(ObjectPointer op, MultiTargetAction* action);
		virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);

		virtual void merge(MultiTargetAction* second);

	protected:
		// Note: Actions are "remembered", so they must 
		// be forgotten when removing from this map.
		map<objId, MultiTargetAction*> actionMap;
	};
}

#endif

