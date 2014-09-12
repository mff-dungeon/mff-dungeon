#ifndef DROPPER_HPP
#define	DROPPER_HPP

#include "common.hpp"
#include "IObject.hpp"
#include "Objects/Location.hpp"
#include "ObjectPointer.hpp"

namespace Dungeon {

	/**
	 * Used for drop table, represents each possible drop for a creature
	 */
	class Dropper : public IObject {
	public:
		enum DropChance {
			Always = 1000000,
			Half = 500000,
			Percent = 10000,
			Thousandth = 1000,
			Milionth = 1
		};
		Dropper();
		Dropper(objId id);
		virtual ~Dropper();
		
		ObjectPointer getItem();
		Dropper* setItem(ObjectPointer item);
		int getChance() const;
		Dropper* setChance(int chance);
		int getMin() const;
		Dropper* setMin(int min);
		int getMax() const;
		Dropper* setMax(int max);
		
		/**
		 * Calculates a drop and creates that item, if it succeeds
		 * @param loc Location to put the drop to
		 * @return true, if it dropped something, false otherwise
		 */
		bool tryDrop(ObjectPointer loc);

		virtual void registerProperties(IPropertyStorage& storage);

		virtual void getActions(ActionList* list, ObjectPointer callee);
		
	private:
		int chance = 0; // 1000000 = 100%
		int min = 1;
		int max = 1;
		
	PERSISTENT_DECLARATION(Dropper, IObject)
	};
}

#endif	/* DROPPER_HPP */

