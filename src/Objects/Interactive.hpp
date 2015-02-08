#ifndef INTERACTIVE_HPP
#define	INTERACTIVE_HPP

#include "../common.hpp"
#include "Virtual/IDescriptable.hpp"

namespace Dungeon {

    class Interactive : public IDescriptable {
    public:

        Interactive()
        {
        }
        
        Interactive(objId id) : IDescriptable(id)
        {
        }

        virtual ~Interactive()
        {
        }

        string getContent() const
        {
            return content.length() ? content : "Greetings %u.";
        }

        Interactive* setContent(string content)
        {
            this->content = content;
            return this;
        }

        virtual void registerProperties(IPropertyStorage& storage);

        PERSISTENT_DECLARATION(Interactive, IDescriptable)

        class BasicInteraction : public Dungeon::MultiTargetAction {
        public:
            BasicInteraction(string type, string verb, string explanation);

            virtual void explain(ActionDescriptor* ad);
            virtual bool match(string command, ActionDescriptor* ad);
            virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);

        private:
            string verb, explanation;
        };

    private:
        string content;
    };
}

#endif

