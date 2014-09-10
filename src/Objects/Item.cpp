#include <cstring>

#include "Item.hpp"
#include "../Actions/CallbackAction.hpp"
#include "../ActionDescriptor.hpp"
#include "../RandomString.hpp"
#include "../SentenceJoiner.hpp"

namespace Dungeon {
	
	Item::Item() {
		this->setDropable(true)->setPickable(true)->setSize(0)->setWeight(0);
	}
	
	Item::Item(objId id) : IDescriptable(id) {
		this->setDropable(true)->setPickable(true)->setSize(0)->setWeight(0);
	}

	Item::~Item() {

	}
	
	Item* Item::setSize(int size) {
		this->size = size;
		return this;
	}
	
	int Item::getSize() const {
		return this->size;
	}
	
	Item* Item::setWeight(int weight) {
		this->weight = weight;
		return this;
	}
	
	int Item::getWeight() const {
		return this->weight;
	}

	bool Item::isDropable() const {
		return dropable;
	}

	Item* Item::setDropable(bool dropable) {
		this->dropable = dropable;
		return this;
	}

	bool Item::isPickable() const {
		return pickable;
	}

	Item* Item::setPickable(bool pickable) {
		this->pickable = pickable;
		return this;
	}

	void Item::getActions(ActionList* list, ObjectPointer callee) {
		ExamineEction* ex = new ExamineEction();
		ex->addTarget(this);
		list->addAction(ex);
	}
	
	void Item::examine(ActionDescriptor* ad) {
		triggerTraps("examine", ad);
		if (ad) {
			*ad << getDescription();
		}
	}
	
	string Item::getDescription() const {
		stringstream ss;
		ss << IDescriptable::getDescription();
		if (getWeight())
			ss << "It weights " << Utils::weightStr(getWeight()) << ". ";
		else ss << "It weights almost nothing. "; 
		if (!isPickable())
			ss << "It cannot be taken. ";
		if (!isDropable())
			ss << "Once taken, it cannot be dropped. ";
		return ss.str();
	}

	
	string Item::getDescriptionSentence() {
		return getGroupDescriptionSentence({ this });
	}
	
	string Item::getGroupDescriptionSentence(vector<ObjectPointer> others) {
		SentenceJoiner items;
		for (auto i = others.begin(); i != others.end(); i++) {
			items << *i;
		}

		return RandomString::get()
				<< items.getSentence("", "You see % lying on the ground. ", "You see %. ") << endr
				<< items.getSentence("", "There lies %. " , "There lies % on the ground. ") << endr
				<< items.getSentence("", "% lies nearby. ", "On the ground there are %. ") << endr;
	}
	
	void Item::registerProperties(IPropertyStorage& storage) {
		storage.have(size, "item-size", "Item size")
			.have(weight, "item-weight", "Item weight")
			.have(pickable, "item-pickable", "Item Pick-ability")
			.have(dropable, "item-dropable", "Item Drop-ability");
		IDescriptable::registerProperties(storage);
	}
	
	PERSISTENT_IMPLEMENTATION(Item)
			
		
	bool ExamineEction::matchCommand(string command) {
		return RegexMatcher::match("examine .+", command);
	}	

	void ExamineEction::commit(ActionDescriptor* ad) {
		commitOnBestTarget(ad, ad->in_msg.substr(8));
	}

	void ExamineEction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		target.assertType<Item>("Examining not-an item.")
				.unsafeCast<Item>()->examine(ad);
	}



}
