#include <string>
#include "../Objects/Virtual/Base.hpp"

#ifndef ADDIOBJECT_HPP
#define	ADDIOBJECT_HPP

namespace Dungeon {

	/* 
	 * @magic
	 * This class is a simple class 
	 * for static registration of objects into ObjectList
	 */
	
    class Base;

	class AddObject {
	private:
		AddObject() {}
	public:
		AddObject(const string& className, const Base* o);
	};
}
#endif

