#include <string>
#include "IObject.hpp"
#include "ObjectList.hpp"

#ifndef ADDIOBJECT_HPP
#define	ADDIOBJECT_HPP

using namespace std;

/* 
 * This class is a simple class 
 *  for static registration of objects into ObjectList
 */

namespace Dungeon {

	class AddIObject { // Class for static registration
	private:
		AddIObject();
		AddIObject(const AddIObject&);
		AddIObject& operator=(const AddIObject&);
	public:
		AddIObject(string className, const IObject* o);
	};
}
#endif

