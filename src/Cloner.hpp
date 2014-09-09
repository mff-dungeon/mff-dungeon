#ifndef CLONER_HPP
#define	CLONER_HPP

#include "ObjectPointer.hpp"
#include "IPropertyStorage.hpp"
#include "GameManager.hpp"
#include <vector>

namespace Dungeon {
	/**
	 * A class used for cloning game objects
	 * Clones all properties of the items, doesn't clone ID
     */
	class Cloner : IPropertyStorage {
	public:
		Cloner(ObjectPointer original, GameManager* gm) : orig(original), gm(gm) {}
		
		ObjectPointer getClone();
		/**
		 * Method used to clone any object. Creates a new Object, clones its properties and stores it in DB
         * @param original the object to be cloned
         * @param gm GameManager
         * @return an ObjectPointer to a new object
         */
		static ObjectPointer clone(ObjectPointer original, GameManager* gm);
		
		virtual IPropertyStorage& have(bool& prop, string id, string desc, bool editable);
		virtual IPropertyStorage& have(int& prop, string id, string desc, bool editable);
		virtual IPropertyStorage& have(string& prop, string id, string desc, bool editable);
		
	private:		
		ObjectPointer orig;
		IObject* cloned;
		GameManager* gm;
		std::queue<void*> properties;
		bool storing = false;
	};
}

#endif	/* CLONER_HPP */

