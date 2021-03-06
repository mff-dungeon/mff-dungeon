#include "Base.hpp"
#include "../../Persistence/ObjectList.hpp"
#include "../../Persistence/Archiver.hpp"
#include "../../Persistence/Cloner.hpp"
#include "../../Game/ObjectPointer.hpp"
#include "../../Game/GameManager.hpp"
#include "../Traps/Trap.hpp"
#include "../Behavior/Behavior.hpp"
#include <memory>

#include <stdexcept>

namespace Dungeon {

	const objId& Base::getId() const {
		return id;
	}

	Base* Base::setId(objId id) {
		this->id = id;
		return this;
	}

	string Base::getObjectType() const {
		string type = getId();
		if(type.find("template/") == 0) type = type.substr(9);
		return type.substr(0, type.rfind("/"));
	}

	void Base::getActionsRecursive(ActionDescriptor *ad) {
		triggerTraps("get-actions", nullptr);
		delegateGetActions(ad, R_BEHAVIOR);
	}

	void Base::delegateGetActions(ActionDescriptor *ad, std::initializer_list<const string> relations) const {
		ad->delegationStack.push(this);
		for (auto& type : relations) {
			for (auto& rel : getRelations(Relation::Master, type)) {
				LOGS(Debug) << getId() << " delegating by " << type << " to " << rel.first << LOGF;
				rel.second->getActionsRecursive(ad);
			}
		}
		if (ad->delegationStack.top() != this) {
			LOGS(Error) << "Some piece of shitty code broken the delegation stack. Dump:" << LOGF;
			while (!ad->delegationStack.empty()) {
				LOGS(Debug) << " - " << ad->delegationStack.top().getId() << LOGF;
				ad->delegationStack.pop();
			}
			throw logic_error("Some piece of shitty code broken the delegation stack.");
		}
		ad->delegationStack.pop();
	}

	void Base::delegateGetActions(ActionDescriptor* ad, const string& relation) const {
		delegateGetActions(ad, { relation });
	}

	void Base::store(Archiver& stream, objId& oid, string& className) const {
		className = this->className();
		oid = this->getId();

		stream.setDirection(true);
		const_cast<Base*>(this)->registerProperties(stream);
	}

	Base* Base::load(Archiver& stream, const string& className) {
		Base* clone = ObjectList::getInstance().create(className);
		if(clone == 0) {
			LOGS(Error) << "Error creating object of type " << className << "." << LOGF;
			throw runtime_error("Object::Base::load: Error creating object.");
		}

		stream.setDirection(false);
		clone->registerProperties(stream);
		return clone;
	}

	bool Base::hasRelation(const string& type, ObjectPointer other, Relation::Dir dir) {
		try {
		LOGS(Debug) << "Checking relation " << getId() << (dir ? "-->" : "<--") << other.getId() << " type " << type << LOGF;
			if(dir == Relation::Master) {
				return relation_master[type].find(other.getId()) != relation_master[type].end();
			} else {
				return relation_slave[type].find(other.getId()) != relation_slave[type].end();
			}
		} catch (std::out_of_range& oor) {
			return false; // No relation of this type
		}
	}

	void Base::addRelation(const string& type, ObjectPointer other, Relation::Dir dir){
		if (hasRelation(type, other, dir)) return;

		LOGS(Debug) << "Adding relation " << getId() << (dir ? " <-- " : " --> ") << other.getId() << " of type " << type << LOGF;
		if(dir) {
			relation_master[type][other.getId()] = other;
		}
		else {
			relation_slave[type][other.getId()] = other;
		}

		if (other.isLoaded())
			other->addRelation(type, this, !dir);
	}

	const RelationList& Base::getRelations(Relation::Dir dir) const {
		return dir == Relation::Dir::Master ? relation_master : relation_slave;
	}

	const ObjectMap& Base::getRelations(Relation::Dir dir, const string& type) const {
		try {
			return getRelations(dir).at(type);
		} catch (std::out_of_range& e) {
			/*
			 * The method is actually const, because the data structure is same on the outside
			 * The const_cast is used to insert an empty map to be able to return ObjectMap reference
			 * Prevents unneccesary copying and still allows being called in const methods
			 */
			if(dir == Relation::Master) {
				const_cast<Base*>(this)->relation_master.insert(pair<string,ObjectMap>(type, ObjectMap()));
			}
			else  {
				const_cast<Base*>(this)->relation_slave.insert(pair<string,ObjectMap>(type, ObjectMap()));
			}
			return getRelations(dir).at(type);
		}
	}

	ObjectPointer Base::getSingleRelation(const string& type, Relation::Dir dir, const string& errMsg) const {
		try {
			const ObjectMap& objects = getRelations(dir, type);
			if (objects.size() > 1)
				throw GameStateInvalid(errMsg);
			if (objects.size() == 0)
				return ObjectPointer();
			return objects.begin()->second;
		} catch (std::out_of_range& e) {
			// no relation of this type ever existed
			return ObjectPointer();
		}
	}

	ObjectPointer Base::setSingleRelation(const string& type, ObjectPointer other, Relation::Dir dir, const string& errMsg) {
		if(dir == Relation::Master) {
			gm->removeRelation(this, getSingleRelation(type, dir, errMsg), type);
			if (!!other) gm->createRelation(this, other, type);
		}
		else {
			gm->removeRelation(getSingleRelation(type, dir, errMsg), this, type);
			if (!!other) gm->createRelation(other, this, type);
		}
		return this;
	}


	void Base::eraseRelation(const string& type, ObjectPointer other, Relation::Dir dir) {
		if (!other || !hasRelation(type, other, dir)) return;

		LOGS(Debug) << "Erasing relation " << getId() << (dir ? " <-- " : " --> ") << other.getId() << " of type " << type << LOGF;
		if(dir) {
			relation_master.at(type).erase(other.getId());
		}
		else {
			relation_slave.at(type).erase(other.getId());
		}

		if (other.isLoaded())
			other->eraseRelation(type, this, !dir);
	}

	void Base::registerProperties(IPropertyStorage& storage) {
		// There can also be an object without internal variables,
		// and serves as a base to recursion
	}

	ObjectPointer Base::save() {
		gm->saveObject(this);
		return this;
	}

	GameManager* Base::getGameManager() const {
		return gm;
	}

	void Base::setGameManager(GameManager* gm) {
		this->gm = gm;
	}

	ObjectPointer Base::triggerTraps(const string& event, ActionDescriptor* ad) {
		try {
			LOGS(Debug) << "Triggering event " << event << " on " << id << ":" << LOGF;
			// Intentionally copying - relations usually change in traps
			const ObjectMap map = getRelations(Relation::Slave, Trap::getRelation(event));
			for (const ObjectMap::value_type& pair : map) {
				LOGS(Debug) << "	trap " << pair.second.getId() << LOGF;
				pair.second.safeCast<Trap>()->trigger(event, this, ad);
			}
		} catch (std::out_of_range& e) {}
		return this;
	}

	ObjectPointer Base::attachTrap(ObjectPointer trap, const string& event) {
		trap.assertType<Trap>("Tried to attach non-trap");
		gm->createRelation(trap, this, Trap::getRelation(event));
		return this;
	}

	ObjectPointer Base::detachTrap(ObjectPointer trap, const string& event) {
		trap.assertType<Trap>("Tried to detach non-trap");
		gm->removeRelation(trap, this, Trap::getRelation(event));
		return this;
	}

	bool Base::hasTrapAttached(ObjectPointer trap, const string& event) {
		return trap.assertType<Trap>("Tried to check non-trap attachment.")->hasRelation(Trap::getRelation(event), this);
	}

	ObjectPointer Base::deepClone() const {
		return Cloner::deepClone(this);
	}

	ObjectPointer Base::shallowClone() const {
		return Cloner::shallowClone(this);
	}

	NONPERSISTENT_IMPLEMENTATION(Base, Base)
}
