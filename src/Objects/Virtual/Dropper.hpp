#ifndef DROPPER_HPP
#define	DROPPER_HPP

#include "../../common.hpp"
#include "Base.hpp"
#include "../../Game/ObjectPointer.hpp"
#include "../../Utils/SentenceJoiner.hpp"
#include "../../Game/ObjectGroup.hpp"

namespace Dungeon {

	/**
	 Defines a custom percent literal, _p
	 */
	constexpr int operator"" _p ( long double value ) {
		return (int)(value * 10000);
	}
	/**
	 * Used for drop table, represents each possible drop for a creature
	 */
	class Dropper : public Base {
	public:

		enum DropChance {
			Always = 100.0_p,
			Half = 50.0_p,
			Percent = 1.0_p,
			Thousandth = 0.1_p,
			Milionth = 0.0001_p
		};
		Dropper();
		Dropper(const objId& id);
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
                 *
                 * FIXME: Remove info, use the group returned, or just make it better :)
		 */
		ObjectGroup tryDrop(ObjectPointer loc, SentenceJoiner& info);

		virtual void registerProperties(IPropertyStorage& storage);

		virtual void getActions(ActionDescriptor* ad) {}

	private:
		int chance = 0; // 1000000 = 100_p = 100%
		int min = 1;
		int max = 1;

		PERSISTENT_DECLARATION(Dropper, Base)
	};
}

#endif	/* DROPPER_HPP */

