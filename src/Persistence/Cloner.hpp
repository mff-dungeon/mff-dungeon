#ifndef CLONER_HPP
#define	CLONER_HPP

#include "../Game/ObjectPointer.hpp"
#include "IPropertyStorage.hpp"
#include <queue>

namespace Dungeon {
	/**
	 * A class used for cloning game objects
	 * Clones all properties of the items, doesn't clone ID
     */
	class Cloner : IPropertyStorage {
	public:
		Cloner(ObjectPointer original) : orig(original) { }

		ObjectPointer getShallowClone();
		ObjectPointer getDeepClone();
		/**
		 * Method used to clone any object. Creates a new Object, clones its properties and stores it in DB
         * @param original the object to be cloned
         * @param gm GameManager
         * @return an ObjectPointer to a new object
         */
		static ObjectPointer shallowClone(ObjectPointer original);
		/**
		 * Method used to clone any object. Creates a new Object, clones its properties, stores it in DB and
		 * clones its relations.
         * @param original the object to be cloned
         * @param gm GameManager
         * @return an ObjectPointer to a new object
         */
		static ObjectPointer deepClone(ObjectPointer original);

		virtual IPropertyStorage& have(bool& prop, const string& id, const string& desc, bool editable);
		virtual IPropertyStorage& have(int& prop, const string& id, const string& desc, bool editable);
		virtual IPropertyStorage& have(long& prop, const string& id, const string& desc, bool editable);
		virtual IPropertyStorage& have(string& prop, const string& id, const string& desc, bool editable);

	private:
		ObjectPointer orig;
		Base* cloned;
		std::queue<void*> properties;
		bool storing = false;
	};
}

#endif	/* CLONER_HPP */

