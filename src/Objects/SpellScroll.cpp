#include "SpellScroll.hpp"
#include "../Utils/RegexMatcher.hpp"
#include "../Game/GameManager.hpp"
#include "Spells/Spell.hpp"
#include "Human.hpp"

namespace Dungeon {

	SpellScroll::SpellScroll() { }

	SpellScroll::SpellScroll(const objId& id) : Item(id) { }

	SpellScroll::~SpellScroll() { }

	SpellScroll* SpellScroll::setSpell(ObjectPointer spell) {
		spell.assertExists("What will be that scroll doing again? Operating a black hole?")
				.assertType<Spell>("Oh, a spell scroll should teach spell, not something else.");
		setSingleRelation("spell-teach", spell, Relation::Master);
		return this;
	}

	ObjectPointer SpellScroll::getSpell() const {
		return getSingleRelation("spell-teach", Relation::Master);
	}

	void SpellScroll::getActions(ActionDescriptor* ad) {
		ad->getActionList().addAction(new ReadScrollAction)
				->addTarget(this)
				->useActionFor(this, ad);
	}

	PERSISTENT_IMPLEMENTATION(SpellScroll)

	void ReadScrollAction::explain(ActionDescriptor* ad) {
		*ad << "Use 'read ...' to read the scroll." << eos;
	}

	bool ReadScrollAction::match(const string& command, ActionDescriptor* ad) {
		if (RegexMatcher::match("read .+", command)) {
			selectBestTarget(command.substr(5), ad);
			return true;
		}
		return false;
	}

	void ReadScrollAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		Human* reader = ad->getCaller();
		target.assertExists("Are you trying to read air?").assertType<SpellScroll>("You cannot read that.");
		SpellScroll* scroll = target.unsafeCast<SpellScroll>();
		ObjectPointer spell = scroll->getSpell();
		if (!scroll->checkStatReqs(ad->getCaller(), ad)) {
			*ad << "You cannot read this scroll now." << eos;
			return;
		}

		if (reader->knowsSpell(spell)) {
			*ad << "You already learned " << spell.safeCast<IDescriptable>()->getName() << " before." << eos;
		} else {
			reader->learnSpell(spell);
			*ad << "You have learned " << spell.safeCast<IDescriptable>()->getName()
					<< " and the " << scroll->getName() << " has disappeared." << eos;
			ad->getGM()->deleteObject(scroll);
		}
	}
}