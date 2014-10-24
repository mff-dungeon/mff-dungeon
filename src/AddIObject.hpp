#include <string>
#include "IObject.hpp"

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
    class IObject;

	class AddIObject {
	private:
		AddIObject();
		AddIObject(const AddIObject&);
		AddIObject& operator=(const AddIObject&);
	public:
		AddIObject(string className, const IObject* o);
	};
}
#endif

