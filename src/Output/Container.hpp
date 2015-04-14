#ifndef CONTAINER_HPP
#define	CONTAINER_HPP

#include "../common.hpp"
#include "Output.hpp"
#include <sstream>
#include <memory>

namespace Dungeon {
namespace Output {

    class Container : public Base
    {
    public:
        // Can be changed to shared to allow copying
        typedef unique_ptr<Base> ptr_t;
        typedef vector<ptr_t> cont_t;

        // TODO implement std container behavior if its worth it

        virtual string plainString() const;
        virtual void xhtml(Tag * parent) const;

        template<typename T>
        Container& consume(T&& o) {
            T* copy = new T(move(o));
            return insert(copy);
        }

        template<typename T, typename... Args>
        Container& emplace(Args&&... values) {
            contents.push_back(ptr_t(new T(forward<Args>(values)...)));
            return *this;
        }

        Container& insert(Base* o) {
            contents.push_back(ptr_t(o));
            return *this;
        }
        
        Container& insert(ptr_t&& o) {
            contents.push_back(move(o));
            return *this;
        }

        Container& operator<<(Base* o) {
            return insert(o);
        }

        size_t size() const { return contents.size(); }
        bool empty() const { return contents.empty(); }

        virtual void clear() {
            contents.clear();
        }

        virtual void swap(Container& other) {
            contents.swap(other.contents);
        }

    protected:
        cont_t contents;
    };

    class SentenceContainer : public Container
    {
    public:
        virtual string plainString() const;
        virtual void xhtml(Tag * parent) const;          
    };

    class ListContainer : public Container
    {
    public:
        virtual string plainString() const;
        virtual void xhtml(Tag * parent) const;
    };
}
}
#endif

