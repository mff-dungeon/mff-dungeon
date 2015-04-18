#ifndef OUTPUT_HPP
#define	OUTPUT_HPP

#include "../common.hpp"
#include <gloox/tag.h>
#include <memory>

namespace Dungeon {
namespace Output {

    /**
     * For Output usage. We don't want to
     * hardcode it for gloox, though we
     * don't want to reimplement Tag either.
     */
    typedef gloox::Tag Tag;

    /**
     * Base class for output objects
     */
    class Base
    {
    public:
        
        /**
         * In string, block elements should end with a newline.
         * In XHtml, they should be made from block element.
         * 
         * Container will put newline before every first block element
         * in a row, making it look similar to XHtml.
         */
        enum DisplayMode {
            Line,
            Block
        } displayMode = Line;
        
        /**
         * Currently supported output methods.
         */
        virtual string plainString() const = 0;

        /**
         * Caller becomes the owner.
         */
        virtual void xhtml(Tag * parent) const = 0;

        template <typename T, typename... Args>
        static unique_ptr<T> create(Args&&... vals) {
            return unique_ptr<T>(new T(forward<Args>(vals)...));
        }
        
    protected:
    };
    

    /**
     * Just a string.
     */
    class PlainString : public Base
    {
    public:
        PlainString() {}
        PlainString(const string& str) : str(str) {}
        PlainString(string&& str) : str(str) {}

        virtual string plainString() const;

        virtual void xhtml(Tag * parent) const;
        
        static unique_ptr<PlainString> create(const string& str) {
            return Base::create<PlainString>(str);
        }
        
        static unique_ptr<PlainString> create(string&& str) {
            return Base::create<PlainString>(move(str));
        }
        
    protected:
        string str;
    };

    /**
     * Just a string.
     */
    class FormattedString : public PlainString
    {
    public:
        FormattedString() {}
        FormattedString(const string& tag, const string& str) : PlainString(str), tag(tag) {}

        virtual void xhtml(Tag * parent) const;

        static unique_ptr<FormattedString> create(const string& tag, const string& str) {
            return Base::create<FormattedString>(tag, str);
        }
        
    protected:
        string tag;
    };


    /**
     * A newline.
     */
    class Newline : public Base
    {
    public:
        virtual string plainString() const;

        virtual void xhtml(Tag * parent) const;
        
        static unique_ptr<Newline> create() {
            return Base::create<Newline>();
        }
    };
}
}

#endif

