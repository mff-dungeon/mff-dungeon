#ifndef BASICINTERACTION_HPP
#define	BASICINTERACTION_HPP

#include "../../common.hpp"
#include "Behavior.hpp"

namespace Dungeon
{

    class BasicInteraction : public Behavior {
    public:
        BasicInteraction() : Behavior() { }
        BasicInteraction(const objId& id) : Behavior(id) { }

        const string getContent() const
        {
            return content.length() ? content : "Greetings %u.";
        }

        BasicInteraction* setContent(const string& content)
        {
            this->content = content;
            return this;
        }

        virtual void trigger(ObjectPointer target, ActionDescriptor* ad);

        virtual void registerProperties(IPropertyStorage& storage);

        PERSISTENT_DECLARATION(BasicInteraction, Behavior)

    private:
        string content = "";
    };
}

#endif

