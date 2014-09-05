#include "common.hpp"

#ifndef OBJECTPOINTER_HPP
#define	OBJECTPOINTER_HPP

namespace Dungeon {

    struct ObjectPointer {
    public:
        ObjectPointer() : gm(NULL), id("") {} // "Null OP"
        ObjectPointer(const nullptr_t n) :  ObjectPointer() {} // "Null OP"
        ObjectPointer(GameManager *gm, objId id)  : gm(gm), id(id) {}
        ObjectPointer(const ObjectPointer& other) : ObjectPointer(other.gm, other.id) {}
        ObjectPointer(const IObject* ptr) : ObjectPointer(ptr->gm, ptr->id) {}

        virtual ~ObjectPointer() {}
        
        objId getId() const
        {
            return this->id;
        }
        
        bool isLoaded() const;

        /**
         * You can use it either as an IObject*...
         */
        operator IObject* () const {
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
        IObject* operator->() const {
            return get();
        }
        
        /**
         * Perform dynamic cast to type
         * TODO Work out how to do it with instanceOf
         */
        template<typename T>
        inline T* safeCast() const {
            IObject* target = dynamic_cast<T*>(get());
            if (!target)
                LOG("ObjectPointer") << "Tried to cast " + id + " to " + typeid(T).name() + " but that's not possible." << LOGF;
            return (T*) target;
        }
        
        /**
         * Like a static cast, does not check instanceOf and returns proper type
         * Use only after manually checking instanceof
         */
        template<typename T>
        inline T* unsafeCast() const {
            return (T*) get();
        }
        
        template<typename T>
        inline const ObjectPointer& assertType(string msg = "") const {
            if (safeCast<T>() == nullptr)
                throwInvalidType(msg);
            return *this;
        }
        
        /**
         * Needs to be in the cpp file because of circular dependency with exceptions
         * @param msg
         */
        void throwInvalidType(string msg = "") const;
        
        const ObjectPointer& assertExists(string msg = "") const;
        
        const ObjectPointer& assertRelation(string type, ObjectPointer other, Relation::Dir master = Relation::Master, string msg = "") const;
        
        IObject* operator*() const {
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
        
        bool operator==(const IObject* other) const {
            return isLoaded() && get() == other;
        }
        
        bool operator!=(const IObject* other) const {
            return !(*this == other);
        }
        
    protected:
        /**
         * Intentionally protected. 
         * OP shall be used as regular pointer, and type-casted with [un]safeCast().
         */
        IObject *get() const;

    private:
        GameManager *gm;
        objId id;
    };
}

#endif
