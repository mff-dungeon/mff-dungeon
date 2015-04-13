#ifndef THORSHAMMER_HPP
#define	THORSHAMMER_HPP

#include <queue>
#include "../common.hpp"
#include "../Actions/Action.hpp"
#include "../Game/ActionDescriptor.hpp"
#include "Item.hpp"

namespace Dungeon {

	/** 
	 * Allows it's bearer perform administrative tasks. Beware!
	 */
	class ThorsHammer : public Item {
	public:
		ThorsHammer();
		virtual ~ThorsHammer();

		virtual void getActions(ActionDescriptor* ad);

	private:
		/**
		 * TODO his really needs to prevent target from unloading in Splay tree,
		 * otherwise it could do terrible things!
		 */
		class PropertyEditor : public MultiTargetAction, public IPropertyStorage {
		public:
			PropertyEditor() : MultiTargetAction("property-editor") { }

			// Action side
			virtual void explain(ActionDescriptor* ad);
			virtual bool match(const string& command, ActionDescriptor* ad);
			virtual bool handleException(GameException& exception, ActionDescriptor* ad);
			virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);

			// Property Storage
			virtual IPropertyStorage& have(string& prop, const string& id, const string& desc, bool editable);
			virtual IPropertyStorage& have(int& prop, const string& id, const string& desc, bool editable);
			virtual IPropertyStorage& have(long& prop, const string& id, const string& desc, bool editable);
			virtual IPropertyStorage& have(bool& prop, const string& id, const string& desc, bool editable);

			void askForNextOne(ActionDescriptor* ad);

		private:
			ActionDescriptor* ad;
			ObjectPointer target;
			std::queue<string> descriptions;
		};

		PERSISTENT_DECLARATION(ThorsHammer, Item)
	};
}
#endif	/* THORSHAMMER_HPP */

