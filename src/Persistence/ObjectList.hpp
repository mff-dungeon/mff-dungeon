#include <map>
#include <string>
#include "../Objects/Virtual/Base.hpp"

#ifndef OBJECTLIST_HPP
#define	OBJECTLIST_HPP

using namespace std;


namespace Dungeon {

	/*
	 * Class holding the prototypes of each object type.
	 * Each object registers here with the macros.
	 * This object can then return a new instance using the create method.
	 * Implemented as singleton to avoid using multiple objects of this type.
	 */
	class ObjectList {
	private:
		map<string, const Base*> names;
		ObjectList() { } // Private constructor
		~ObjectList();

	public:
		static ObjectList& getInstance();
		void addIObject(string className, const Base* o);
		Base* create(string className);
	};
}
#endif