#ifndef OUTPUT_HPP
#define	OUTPUT_HPP

#include "../common.hpp"
#include <gloox/tag.h>

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
        Base() {}

        /**
         * Currently supported output methods.
         */
        virtual string plainString() const = 0;

        /**
         * Caller becomes the owner.
         */
        virtual void xhtml(Tag * parent) const = 0;

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

        virtual string plainString() const;

        virtual void xhtml(Tag * parent) const;
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
    };
}
}

#endif

