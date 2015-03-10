#ifndef IDESCRIPTABLE_HPP
#define	IDESCRIPTABLE_HPP

#include "../../common.hpp"
#include "../../Utils/RegexMatcher.hpp"
#include "../../Actions/MultiTargetAction.hpp"
#include "Base.hpp"

namespace Dungeon {

	/**
	 * An object with a name and some description. 
	 * In fact, every object should be descriptable, 
	 * but the functionality is split better this way.
	 */
	class IDescriptable : public Base {
	public:
		IDescriptable() : Base() { }
		IDescriptable(const objId& id) : Base(id) { }

		/**
		 * Description string. Will be displayed hile exploring room.
		 */
		virtual string getDescription() const;
		IDescriptable* setDescription(const string& description);

		/**
		 * Long name is used mainly for matching.
		 */
		virtual string getLongName() const;
		IDescriptable* setLongName(const string& longName);

		/**
		 * Short name is used when there are many things 
		 * and we need to spare some bytes :)
		 */
		virtual string getName() const;
		IDescriptable* setName(const string& name);

		/**
		 * Describes this item in explore reply
		 */
		virtual string getDescriptionSentence();

		/**
		 * When there are more instances of this class, 
		 * we can produce better summary.
		 */
		virtual string getGroupDescriptionSentence(vector<ObjectPointer> others);

		virtual void getActions(ActionList* list, ObjectPointer callee);
		virtual void registerProperties(IPropertyStorage& storage);

		virtual void examine(ActionDescriptor* ad);

		NONPERSISTENT_DECLARATION(IDescriptable, Base)

	private:
		string name = "", longName = "";
		string description = "";
	};

	class ExamineEction : public MultiTargetAction {
	public:
		ExamineEction() : MultiTargetAction("examine") { }

		virtual bool match(const string& command, ActionDescriptor* ad);
		virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);
		virtual void explain(ActionDescriptor* ad);
	};
}

#endif

