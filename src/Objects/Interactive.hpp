#ifndef INTERACTIVE_HPP
#define	INTERACTIVE_HPP

#include "../common.hpp"
#include "Virtual/IDescriptable.hpp"
#include "Traps/Trap.hpp"

namespace Dungeon {

    /**
     * If you look closer on the implementation, you'll find out that 
     * Interactive is just a syntax sugar. Interactive action can be 
     * added to virtually any object in the game. But, as always, be careful.
     */
    class Interactive : public IDescriptable {
    public:
        Interactive() : IDescriptable() { }
        Interactive(objId id) : IDescriptable(id) { }
        virtual ~Interactive() { }
        
        Interactive* reads(const string& content) {
            attachInteraction("read", "read ... - to read it", content);
            return this;
        }
        
        Interactive* says(const string& content) {
            attachInteraction("speak( (with|to))?", "read ... - to read it", content);
            return this;
        }
        
        void attachInteraction(const string& verb, const string& explanation, const string& content);

        PERSISTENT_DECLARATION(Interactive, IDescriptable)

        class BasicInteractionAction : public Dungeon::MultiTargetAction {
        public:
            BasicInteractionAction(const string& type, ObjectPointer interactionTrap);

            virtual void explain(ActionDescriptor* ad);
            virtual bool match(const string& command, ActionDescriptor* ad);
            virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);

        private:
            ObjectPointer interactionTrap;
        };
        
        class BasicInteraction : public Dungeon::Trap {
        public:
            BasicInteraction() : Trap() { }

            BasicInteraction(string id) : Trap(id) { }

            const string getContent() const
            {
                return content.length() ? content : "Greetings %u.";
            }

            BasicInteraction* setContent(const string& content)
            {
                this->content = content;
                return this;
            }
            
            const string& getExplanation() const
            {
                return explanation;
            }

            BasicInteraction* setExplanation(const string& explanation)
            {
                this->explanation = explanation;
                return this;
            }

            const string& getVerb() const
            {
                return verb;
            }

            BasicInteraction* setVerb(const string& verb)
            {
                this->verb = verb;
                return this;
            }

            virtual void trigger(const string& event, ObjectPointer target, ActionDescriptor* ad);

            virtual void registerProperties(IPropertyStorage& storage);
            
            PERSISTENT_DECLARATION(BasicInteraction, Trap)
            
        private:
            string verb = "", explanation = "", content = "";
        };

    private:
        string content;
    };
}

#endif

