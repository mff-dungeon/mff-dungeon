#include "../common.hpp"
#include <memory>

#ifndef OBJECTPOINTER_HPP
#define	OBJECTPOINTER_HPP

namespace Dungeon {

    /**
     * Kind of a proxy class for pointer to Object. Semantics:
     * 
     * If strong_obj is not null, this pointer behaves as a lock and the object
     * should not be deleted for any reason.
     * 
     * If weak_obj can be locked, it will be used to access the object.
     * 
     * Else, the gm and id will be used to load new weak_obj from the object 
     * storage.
     */
    struct ObjectPointer {
    friend class GameManager;
    public:
        typedef std::shared_ptr<Base> ptr_t;
        typedef std::weak_ptr<Base> weak_ptr_t;

        ObjectPointer() : gm(nullptr), id("") {} // "Null OP"
        ObjectPointer(const nullptr_t n) : ObjectPointer() {} // "Null OP"
        
        ObjectPointer(const Base* ptr) : ObjectPointer(ptr->gm, ptr->id) {
            LOGS(Debug) << "Ptr const" << LOGF;
        }
        
        ObjectPointer(const ObjectPointer& other) : gm(other.gm), id(other.id), weak_obj(other.weak_obj) {
            LOGS(Debug) << "Copy const" << LOGF;
        }
        
        ObjectPointer(ObjectPointer&& other) : gm(other.gm), id(move(other.id)), weak_obj(other.weak_obj) {
            LOGS(Debug) << "Move const" << LOGF;
        }

        ObjectPointer& operator=(const ObjectPointer& other) {
            gm = other.gm;
            weak_obj = other.weak_obj;
            strong_obj.reset();
            id = other.id;
            return *this;
        }

        ObjectPointer& operator=(ObjectPointer&& other) {
            gm = other.gm;
            weak_obj = other.weak_obj;
            strong_obj.reset();
            id = move(other.id);
            return *this;
        }
        
        ObjectPointer(const weak_ptr_t ptr) : gm(nullptr), id(""), weak_obj(ptr) {
            ptr_t lptr = weak_obj.lock();
            if (!!lptr) {
                gm = lptr->getGameManager();
                id = lptr->getId();
            }
        }
        
        ObjectPointer(const ptr_t ptr) : ObjectPointer(weak_ptr_t(ptr)) {}

        const objId& getId() const
        {
            return this->id;
        }
        
        bool isLoaded() const;

        /**
         * You can use it either as an Base*...
         */
        operator ptr_t () const {
            return get();
        }
        
        operator Base* () const {
            return get().get();
        }
        
        /**
         * ... or as id ...
         */
        operator const char* () {
            return getId().c_str();
        }
        
        /**
         * .. or as a pointer.
         */
        ptr_t operator->() const {
            return get();
        }
        
        void setLock(bool lock = true) {
            if (lock)
                strong_obj = get();
            else 
                strong_obj.reset();
        }
        
        /**
         * Perform dynamic cast to type. 
         * TODO Work out how to do it with instanceOf
         * @return nullptr, if the object doesn't exist
         */
        template<typename T>
        inline T* safeCast() const {
            try {
                T* target = dynamic_cast<T*>(get().get());
                if (!target)
                    LOG << "Tried to cast " + id + " to " + typeid(T).name() + " but that's not possible." << LOGF;
                return target;
            } catch (ObjectLost& exception) {
                LOG << "Safe-casted non-existing object id " << id << LOGF;
                return nullptr;
            }
        }
        
        /**
         * Like a static cast, does not check instanceOf and returns proper type
         * Use only after manually checking instanceof
         */
        template<typename T>
        inline T* unsafeCast() const {
            return (T*) get().get();
        }
        
        /**
         * Requires the object to exist and to be of given type
         * @throws InvalidType
         */
        template<typename T>
        inline const ObjectPointer& assertType(string msg = "") const {
            if (safeCast<T>() == nullptr)
		throw InvalidType(msg);
            return *this;
        }
        
        /**
         * Requires the object to exist
         * @throws ObjectLost
         */
        const ObjectPointer& assertExists(string msg = "") const;
        
        /**
         * Requires the given relation to exist
         * @throws GameStateChanged
         */
        const ObjectPointer& assertRelation(const string& type, ObjectPointer other, Relation::Dir master = Relation::Master, string msg = "") const;
        
        ptr_t operator*() const {
            return get();
        }
        
        bool operator!() const {
            return gm == NULL;
        }
        
        bool operator==(const ObjectPointer& other) const {
            return other.id == id;
        }
        
        bool operator!=(const ObjectPointer& other) const {
            return other.id != id;
        }
        
        bool operator==(const ptr_t other) const {
            return get() == other;
        }
        
        bool operator!=(const ptr_t other) const {
            return !(*this == other);
        }
        
        bool operator==(const Base* other) const {
            return get().get() == other;
        }
        
        bool operator!=(const Base* other) const {
            return !(*this == other);
        }
        
        bool operator==(const nullptr_t other) const {
            return this->id == "";
        }
        
        bool operator!=(const nullptr_t other) const {
            return this->id != "";
        }
        
    protected:
        /**
         * Intentionally protected. 
         * OP shall be used as regular pointer, and type-casted with [un]safeCast().
         */
	ptr_t get() const;
        
        /**
         * Only GM should use this constructor to create new OP.
         */
        ObjectPointer(GameManager *gm, const objId& id)  : gm(gm), id(id) {}

    private:
        GameManager *gm;
        objId id;
		weak_ptr_t weak_obj;
		ptr_t strong_obj;
    };
}

inline ostream& operator << (ostream& ostream, Dungeon::ObjectPointer obj) {
    ostream << obj.getId();
    return ostream;
}

#endif
