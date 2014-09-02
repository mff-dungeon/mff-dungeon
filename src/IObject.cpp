#include "IObject.hpp"
#include "ObjectPointer.hpp"
#include "GameManager.hpp"
#include <memory>

#include <stdexcept>

namespace Dungeon {

	objId IObject::getId() const {
		return id;
	}

	IObject * IObject::setId(objId id) {
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
			other.get()->addRelation(type, getObjectPointer(), !master);
	}
	
	RelationList IObject::getRelations(bool master) {
		if(master) {
			return relation_master;
		}
		else {
			return relation_slave;
		}
	}
	
	void IObject::eraseRelation(string type, ObjectPointer other, bool master) {
		if (!hasRelation(type, other, master)) return;
		
		LOGS("IObject", Verbose) << "Erasing relation " << getId() << (master ? "<--" : "-->") << other.getId() << " type " << type << LOGF;
		if(master) {
			relation_master.at(type).erase(other.getId());
		}
		else {
			relation_slave.at(type).erase(other.getId());
		}
		
		if (other.isLoaded())
			other.get()->eraseRelation(type, getObjectPointer(), !master);
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