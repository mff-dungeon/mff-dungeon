#include "SpellScroll.hpp"
#include "Human.hpp"
#include "Spell.hpp"

namespace Dungeon {
	
	SpellScroll::SpellScroll() {
		
	}

	SpellScroll::SpellScroll(objId id) : Item(id) {
		
	}

	SpellScroll::~SpellScroll() {
		
	}

	SpellScroll* SpellScroll::setSpell(ObjectPointer spell) {
		spell.assertExists("What will be that scroll doing again? Operating a black hole?")
				.assertType<Spell>("Oh, a spell scroll should teach spell, not something else.");
		setSingleRelation("spell-teach", spell, Relation::Master);
		return this;
	}

	ObjectPointer SpellScroll::getSpell() const {
		return getSingleRelation("spell-teach", Relation::Master);
	}

	void SpellScroll::getActions(ActionList* list, ObjectPointer callee) {
		Item::getActions(list, callee);
	
		list->addAction(new ReadScrollAction)
			->addTarget(this)
			->useActionFor(this, list);
	}
	
	PERSISTENT_IMPLEMENTATION(SpellScroll)

	void ReadScrollAction::explain(ActionDescriptor* ad) {
		*ad << "Use 'read ...' to read the scroll." << eos;
	}
	
	bool ReadScrollAction::match(string command, ActionDescriptor* ad) {
		if (RegexMatcher::match("read .+", command)) {
			selectBestTarget(command.substr(5), ad);
			return true;
		}
		return false;
	}
        
	void ReadScrollAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		Human* reader = ((Human*) ad->getAlive());
		target.assertExists("Are you reading air?").assertType<SpellScroll>("You cannot read this.");
		SpellScroll* scroll = target.unsafeCast<SpellScroll>();
		ObjectPointer spell = scroll->getSpell();
		
		if(reader->knowsSpell(spell)) {
			*ad << "You already learned " << spell.safeCast<IDescriptable>()->getName() << "." << eos;
		}
		else {
			reader->learnSpell(spell);
			*ad << "You have learnt " << spell.safeCast<IDescriptable>()->getName() 
					<< " and the " << scroll->getName() << " has disappeared." << eos;
			ad->getGM()->deleteObject(scroll);
		}
	}
}