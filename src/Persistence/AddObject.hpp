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
	
	/*
	 * This class needs to know that IObject exists and IObject needs to use 
	 * this in its macro
	 */
    class Base;

	class AddObject {
	private:
		AddObject();
		AddObject(const AddObject&);
		AddObject& operator=(const AddObject&);
	public:
		AddObject(string className, const Base* o);
	};
}
#endif

