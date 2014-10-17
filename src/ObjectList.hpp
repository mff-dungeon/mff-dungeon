#include <map>
#include <string>
#include "IObject.hpp"

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
		map<string, const IObject*> names;
		ObjectList() { } // Private constructor
		ObjectList(const ObjectList&);
		ObjectList& operator=(const ObjectList&);
		~ObjectList();

	public:
		static ObjectList& getInstance();
		void addIObject(string className, const IObject* o);
		IObject* create(string className);
	};
}
#endif