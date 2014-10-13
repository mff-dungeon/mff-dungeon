#ifndef IDESCRIPTABLE_HPP
#define	IDESCRIPTABLE_HPP

#include "../common.hpp"
#include "../IObject.hpp"
#include "../Actions/MultiTargetAction.hpp"
#include "../RegexMatcher.hpp"

namespace Dungeon {

	/**
	 * An object with a name and some description. 
	 * In fact, every object should be descriptable, 
	 * but the functionality is split better this way.
	 */
	class IDescriptable : public IObject {
	public:
		IDescriptable() : IObject() { }
		IDescriptable(objId id) : IObject(id) { }

		/**
		 * Description string. Will be displayed hile exploring room.
		 */
		virtual string getDescription() const;
		IDescriptable* setDescription(string description);

		/**
		 * Long name is used mainly for matching.
		 */
		virtual string getLongName() const;
		IDescriptable* setLongName(string longName);

		/**
		 * Short name is used when there are many things 
		 * and we need to spare some bytes :)
		 */
		virtual string getName() const;
		IDescriptable* setName(string name);

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

		NONPERSISTENT_DECLARATION(IDescriptable, IObject)

	private:
		string name = "", longName = "";
		string description = "";
	};

	class ExamineEction : public MultiTargetAction {
	public:
		ExamineEction() : MultiTargetAction("examine") { }

		virtual bool match(string command, ActionDescriptor* ad);
		virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);
		virtual void explain(ActionDescriptor* ad);
	};
}

#endif

