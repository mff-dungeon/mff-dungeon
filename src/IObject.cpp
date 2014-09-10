#include "IObject.hpp"
#include "ObjectPointer.hpp"
#include "GameManager.hpp"
#include "Objects/Trap.hpp"
#include "Cloner.hpp"
#include <memory>

#include <stdexcept>

namespace Dungeon {

	objId IObject::getId() const {
		return id;
	}

	IObject* IObject::setId(objId id) {
		this->id = id;
		return this;
	}
	
	void IObject::store(Archiver& stream, objId& oid, string& className) const {
		className = this->className();
		oid = this->getId();

		stream.setDirection(true);
		const_cast<IObject*>(this)->registerProperties(stream);
	}

	IObject* IObject::load(Archiver& stream, string className) {
		IObject* clone = ObjectList::getInstance().create(className);
		if(clone == 0)
			throw "IObject::load: Error creating object.";

		std::auto_ptr<IObject> holder(clone);
		IObject* object = dynamic_cast<IObject*>(clone);
		if(object == 0) {
			throw "IObject::load: Error creating object.";
		}

		stream.setDirection(false);

		object->registerProperties(stream);
		holder.release();
		return object;
	}
	
	bool IObject::hasRelation(string type, ObjectPointer other, Relation::Dir dir) {
		try {
			if(dir == Relation::Master) {
				return relation_master[type].find(other.getId()) != relation_master[type].end();
			} else {
				return relation_slave[type].find(other.getId()) != relation_slave[type].end();
			}
		} catch (std::out_of_range& oor) {
			return false; // No relation of this type
		}
	}
	
	void IObject::addRelation(string type, ObjectPointer other, Relation::Dir dir) {
		if (hasRelation(type, other, dir)) return;
		
		LOGS("IObject", Verbose) << "Adding relation " << getId() << (dir ? "<--" : "-->") << other.getId() << " type " << type << LOGF;
		if(dir) {
			relation_master[type][other.getId()] = other;
		}
		else {
			relation_slave[type][other.getId()] = other;
		}
		
		if (other.isLoaded())
			other->addRelation(type, this, !dir);
	}

	const RelationList& IObject::getRelations(Relation::Dir dir) {
		return dir == Relation::Dir::Master ? relation_master : relation_slave;
	}
	
	const ObjectMap IObject::getRelations(Relation::Dir dir, string type) {
		try {
			return getRelations(dir).at(type);
		} catch (std::out_of_range& e) {
			return ObjectMap();
		}
	}
	
	ObjectPointer IObject::getSingleRelation(string type, Relation::Dir dir, string errMsg) {
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
	
	ObjectPointer IObject::setSingleRelation(string type, ObjectPointer other, Relation::Dir dir, string errMsg) {
		if(dir == Relation::Master) {
			gm->removeRelation(this, getSingleRelation(type, dir, errMsg), type);
			gm->createRelation(this, other, type);
		}
		else {
			gm->removeRelation(getSingleRelation(type, dir, errMsg), this, type);
			gm->createRelation(other, this, type);
		}
		return this;
	}

	
	void IObject::eraseRelation(string type, ObjectPointer other, Relation::Dir dir) {
		if (!other || !hasRelation(type, other, dir)) return;
		
		LOGS("IObject", Verbose) << "Erasing relation " << getId() << (dir ? "<--" : "-->") << other.getId() << " type " << type << LOGF;
		if(dir) {
			relation_master.at(type).erase(other.getId());
		}
		else {
			relation_slave.at(type).erase(other.getId());
		}
		
		if (other.isLoaded())
			other->eraseRelation(type, this, !dir);
	}
	
	void IObject::serialize(Archiver& stream) {
		this->registerProperties(stream);
	}

	void IObject::registerProperties(IPropertyStorage& storage) {
		// There can also be an object without internal variables, 
		// and serves as a base to recursion
	}
	
	ObjectPointer IObject::save() {
		gm->saveObject(this);
		return this;
	}

	GameManager* IObject::getGameManager() const {
		return gm;
	}
	
	void IObject::setGameManager(GameManager* gm) {
		this->gm = gm;
	}
	
	ObjectPointer IObject::triggerTraps(string event, ActionDescriptor* ad) {
		try {
			LOGS("Object", Verbose) << "Triggering event " << event << " on " << id << ":" << LOGF;
			const ObjectMap& map = getRelations(Relation::Slave, Trap::getRelation(event));
			for (const ObjectMap::value_type& pair : map) {
				LOGS("Object", Verbose) << "	trap " << pair.second.getId() << LOGF;
				pair.second.safeCast<Trap>()->trigger(event, this, ad);
			}
		} catch (std::out_of_range& e) {}
		return this;
	}
	
	ObjectPointer IObject::attachTrap(ObjectPointer trap, string event) {
		trap.assertType<Trap>("Tried to attach non-trap");
		gm->createRelation(trap, this, Trap::getRelation(event));
		return this;
	}
	
	ObjectPointer IObject::detachTrap(ObjectPointer trap, string event) {
		trap.assertType<Trap>("Tried to detach non-trap");
		gm->removeRelation(trap, this, Trap::getRelation(event));
		return this;
	}
	
	ObjectPointer IObject::deepClone() const {
		return Cloner::deepClone(this);
	}
	
	ObjectPointer IObject::shallowClone() const {
		return Cloner::shallowClone(this);
	}



	NONPERSISTENT_IMPLEMENTATION(IObject)

}