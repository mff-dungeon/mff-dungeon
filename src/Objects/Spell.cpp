#include "Spell.hpp"
#include "Human.hpp"

namespace Dungeon {

	Spell::Spell() {
		
	}
	
	Spell::Spell(objId id) : IDescriptable(id) {
		
	}

	Spell::~Spell() {

	}

	int Spell::getBaseEffect() const {
		return effect;
	}

	Spell* Spell::setBaseEffect(int effect) {
		this->effect = effect;
		return this;
	}

	int Spell::getBaseManaCost() const {
		return manaCost;
	}

	Spell* Spell::setBaseManaCost(int mana) {
		this->manaCost = mana;
		return this;
	}

	int Spell::getManaCost(ObjectPointer casterPtr) const {
		casterPtr.assertExists("Who is casting that spell? A ghost?")
			.assertType<Human>("Only humans can cast spells.");
		Human* caster = casterPtr.unsafeCast<Human>();
		
		// TODO: Add some crazy formula depending on human stats
		return getBaseManaCost();
	}

	int Spell::getEffect(ObjectPointer casterPtr) const {
		casterPtr.assertExists("Who is casting that spell? A ghost?")
			.assertType<Human>("Only humans can cast spells.");
		Human* caster = casterPtr.unsafeCast<Human>();
		
		// TODO: Add some crazy formula depending on human stats
		return getBaseEffect();
	}

	bool Spell::checkCast(ObjectPointer casterPtr, ActionDescriptor* ad) const {
		casterPtr.assertExists("Who is casting that spell? A ghost?")
			.assertType<Human>("Only humans can cast spells.");
		Human* caster = casterPtr.unsafeCast<Human>();
		
		if(!caster->hasResourceGreaterThan(Resource::ManaShard, getManaCost(casterPtr))) {
			*ad << "You don't have enough mana to cast " << getName() << "." << eos;
			return false;
		}
		// TODO: Add other important checks, or override for Target checks...
		return true;
	}

	void Spell::cast(ObjectPointer casterPtr, ActionDescriptor* ad) {
		*ad << "You've cast " << getName() << " and your hands just sparkled." << eos;
	}

	void Spell::registerProperties(IPropertyStorage& storage) {
		storage.have(manaCost, "spell-mana", "price of the spell")
				.have(effect, "spell-effect", "effect strength of the spell");
		IDescriptable::registerProperties(storage);
	}

	PERSISTENT_IMPLEMENTATION(Spell)
			
	void CastAction::explain(ActionDescriptor* ad) {
		*ad << "Use 'cast ...' to cast a spell." << eos;
	}
	
	bool CastAction::match(string command, ActionDescriptor* ad) {
		if (RegexMatcher::match("cast .+", command)) {
			selectBestTarget(command.substr(5), ad);
			return true;
		}
		return false;
	}
        
	void CastAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		target.assertExists("The spell doesn't exist")
				.assertType<Spell>("You are casting something weird");
		Spell* spell = target.unsafeCast<Spell>();
		
		if(spell->checkCast(ad->getAlive(), ad)) {
			spell->cast(ad->getAlive(), ad);
		}
	}
}

