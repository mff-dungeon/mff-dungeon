#include "../common.hpp"

#ifndef OBJECTPOINTER_HPP
#define	OBJECTPOINTER_HPP

namespace Dungeon {

    struct ObjectPointer {
    friend class GameManager;
    public:
        ObjectPointer() : gm(NULL), id("") {} // "Null OP"
        ObjectPointer(const nullptr_t n) :  ObjectPointer() {} // "Null OP"
        ObjectPointer(const ObjectPointer& other) : ObjectPointer(other.gm, other.id) {}
        ObjectPointer(const Base* ptr) : ObjectPointer(ptr->gm, ptr->id) {}

        virtual ~ObjectPointer() {
            setLock(false);
        }
        
        const objId& getId() const
        {
            return this->id;
        }
        
        bool isLoaded() const;

        /**
         * You can use it either as an Base*...
         */
        operator Base* () const {
            return get();
        }
        
        /**
         * ... or as id ...
         */
        operator const char * () {
            return getId().c_str();
        }
        
        /**
         * .. or as a pointer.
         */
        Base* operator->() const {
            return get();
        }
        
        void setLock(bool lock = true) {
            if (locked != lock) {
				assertExists("Object must exist in order to be locked.");
                LOGS("ObjectPointer", Debug) << "Object " << id << " is now " << (lock ? "locked" : "unlocked") << LOGF;
                if (locked) get()->loadLock++;
                else get()->loadLock--;
                locked = lock;
            }
        }
        
        /**
         * Perform dynamic cast to type. 
         * TODO Work out how to do it with instanceOf
         * @return nullptr, if the object doesn't exist
         */
        template<typename T>
        inline T* safeCast() const {
            try {
                Base* target = dynamic_cast<T*>(get());
                if (!target)
                    LOG("ObjectPointer") << "Tried to cast " + id + " to " + typeid(T).name() + " but that's not possible." << LOGF;
                return (T*) target;
            } catch (ObjectLost& exception) {
                LOG("ObjectPointer") << "Safe-casted non-existing object id " << id << LOGF;
                return nullptr;
            }
        }
        
        /**
         * Like a static cast, does not check instanceOf and returns proper type
         * Use only after manually checking instanceof
         */
        template<typename T>
        inline T* unsafeCast() const {
            return (T*) get();
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
        const ObjectPointer& assertRelation(string type, ObjectPointer other, Relation::Dir master = Relation::Master, string msg = "") const;
        
        Base* operator*() const {
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
        
        bool operator==(const Base* other) const {
            return isLoaded() && get() == other;
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
        Base *get() const;
        
        /**
         * Only GM should use this constructor to create new OP.
         */
        ObjectPointer(GameManager *gm, objId id)  : gm(gm), id(id) {}

    private:
        GameManager *gm;
        objId id;
        bool locked = false;
    };
}

inline ostream& operator << (ostream& ostream, Dungeon::ObjectPointer obj) {
    ostream << obj.getId();
    return ostream;
}

#endif
