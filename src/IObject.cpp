#include "IObject.hpp"
#include "ObjectPointer.hpp"
#include "GameManager.hpp"
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
	
	bool IObject::hasRelation(string type, ObjectPointer other, bool master) {
		if(master) {
			return relation_master[type].find(other.getId()) != relation_master[type].end();
		} else {
			return relation_slave[type].find(other.getId()) != relation_slave[type].end();
		}
	}
	
	void IObject::addRelation(string type, ObjectPointer other, bool master) {
		if (hasRelation(type, other, master)) return;
		
		LOGS("IObject", Verbose) << "Adding relation " << getId() << (master ? "<--" : "-->") << other.getId() << " type " << type << LOGF;
		if(master) {
			relation_master[type][other.getId()] = other;
		}
		else {
			relation_slave[type][other.getId()] = other;
		}
		
		if (other.isLoaded())
			other->addRelation(type, getObjectPointer(), !master);
	}

	RelationList& IObject::getRelations(Relation::Dir dir) {
		return dir == Relation::Dir::Master ? relation_master : relation_slave;
	}
	
	ObjectMap& IObject::getRelations(Relation::Dir dir, string type) {
		return getRelations(dir).at(type);
	}
	
	ObjectPointer IObject::getSingleRelation(string type, Relation::Dir dir, string errMsg) {
		try {
			ObjectMap& objects = getRelations(dir, type);
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
		eraseRelation(type, getSingleRelation(type, dir, errMsg), dir);
		if(dir == Relation::Master) {
			gm->createRelation(getObjectPointer(), other, type);
		}
		else {
			gm->createRelation(other, getObjectPointer(), type);
		}
		return getObjectPointer();
	}

	
	void IObject::eraseRelation(string type, ObjectPointer other, bool master) {
		if (!other || !hasRelation(type, other, master)) return;
		
		LOGS("IObject", Verbose) << "Erasing relation " << getId() << (master ? "<--" : "-->") << other.getId() << " type " << type << LOGF;
		if(master) {
			relation_master.at(type).erase(other.getId());
		}
		else {
			relation_slave.at(type).erase(other.getId());
		}
		
		if (other.isLoaded())
			other->eraseRelation(type, getObjectPointer(), !master);
	}
	
	void IObject::serialize(Archiver& stream) {
		this->registerProperties(stream);
	}

	void IObject::registerProperties(IPropertyStorage& storage) {
		// There can also be an object without internal variables, 
		// and serves as a base to recursion
	}
	
	IObject* IObject::save() {
		gm->saveObject(this);
		return this;
	}
	
	ObjectPointer IObject::getObjectPointer() {
		return gm->getObjectPointer(this->id);
	}

	GameManager* IObject::getGameManager() const {
		return gm;
	}
	
	void IObject::setGameManager(GameManager* gm) {
		this->gm = gm;
	}

	NONPERSISTENT_IMPLEMENTATION(IObject)

}