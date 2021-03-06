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

        /**
         * Container should put newline before each first block element in a row.
         */
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

        Container& operator<<(ptr_t&& ptr) {
            return insert(move(ptr));
        }

        Container& operator<<(const string& str) {
            return insert(PlainString::create(str));
        }
        
        Container& operator<<(string&& str) {
            return insert(PlainString::create(move(str)));
        }

        size_t size() const { return contents.size(); }
        bool empty() const { return contents.empty(); }

        virtual void clear() {
            contents.clear();
        }

        virtual void swap(Container& other) {
            contents.swap(other.contents);
        }

        static unique_ptr<Container> create() {
            return Base::create<Container>();
        }
    protected:
        cont_t contents;
    };

    class SentenceContainer : public Container
    {
    public:
        SentenceContainer() {
            displayMode = Block;
        }
        
        SentenceContainer(SentenceContainer&& move) = default;
        SentenceContainer& operator=(SentenceContainer&& move) = default;
        virtual ~SentenceContainer() {}
        
        virtual string plainString() const;
        virtual void xhtml(Tag * parent) const;
        
        static unique_ptr<SentenceContainer> create() {
            return Base::create<SentenceContainer>();
        }
    };

    class ListContainer : public Container
    {
    public:
        ListContainer() {
            displayMode = Block;
        }
        
        ListContainer(ListContainer&& move) = default;
        ListContainer& operator=(ListContainer&& move) = default;
        virtual ~ListContainer() {}
        
        virtual string plainString() const;
        virtual void xhtml(Tag * parent) const;
        
        static unique_ptr<ListContainer> create() {
            return Base::create<ListContainer>();
        }
    };
}
}
#endif

