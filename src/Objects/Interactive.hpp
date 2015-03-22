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
            attachInteraction("(speak|talk|chat)( (with|to))?", "speak with ... - to speak with someone", content);
            return this;
        }
        
        void attachInteraction(const string& verb, const string& explanation, const string& content);

        PERSISTENT_DECLARATION(Interactive, IDescriptable)

    private:
        string content;
    };
}

#endif

