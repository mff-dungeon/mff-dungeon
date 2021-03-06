#ifndef OBJECTGROUP_HPP
#define	OBJECTGROUP_HPP

#include "../common.hpp"
#include "GameManager.hpp"
#include "../Utils/FuzzyStringMatcher.hpp"
#include "../Utils/SentenceJoiner.hpp"
#include "../Game/ActionDescriptor.hpp"

namespace Dungeon {

	struct objIdTypeComparator {
		bool operator() (const objId& lhs, const objId& rhs) const {
			// ignore stuff after the last slash char
			string ltype = objId_getType(lhs);
			string rtype = objId_getType(rhs);

			return ltype < rtype;
		}
	};


	typedef multimap<objId, ObjectPointer, objIdTypeComparator> ObjectGroupMap;

	/**
	 * Helper class for grouping objects of the same type.
	 *
	 * Implements exploring, and will implement searching for object
	 * best corresponding to given string.
         *
         * FIXME: Just fix me.
	 */
	class ObjectGroup : public ObjectGroupMap {
	public:
		ObjectGroup();
		ObjectGroup(ObjectPointer ptr);
		ObjectGroup(const vector<ObjectPointer>& pointers);
		ObjectGroup(ObjectMap map);

		ObjectGroupMap::iterator insertObject(ObjectPointer ptr);

                // FIXME: remove the ignore flag
		void explore(ActionDescriptor *ad, bool ignoreOfflineUsers = true);

		ObjectPointer match(string name);
	private:
		ObjectGroupMap::value_type getPair(Base *obj);
		ObjectGroupMap::value_type getPair(ObjectPointer ptr);

	};
}

#endif /* OBJECTGROUP_HPP */
