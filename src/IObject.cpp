#include "IObject.hpp"
#include "ObjectPointer.hpp"
#include <memory>

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
		const_cast<IObject*>(this)->serialize(stream);
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

		object->serialize(stream);
		holder.release();
		return object;
	}
	
	void IObject::addRelation(string type, ObjectPointer* other, bool master) {
		if(master) {
			relation_master[type][other->getId()] = other;
		}
		else {
			relation_slave[type][other->getId()] = other;
		}
	}
	
	RelationList IObject::getRelations(bool master) {
		if(master) {
			return relation_master;
		}
		else {
			return relation_slave;
		}
	}
	
	void IObject::serialize(Archiver& stream) {
		// There can also be an object without internal variables
	}

}