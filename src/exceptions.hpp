#include <exception>
#include "common.hpp"

#ifndef EXCEPTIONS_HPP
#define	EXCEPTIONS_HPP

namespace Dungeon {
    
    /**
     * Base class for in-game exceptions
     */
    class GameException : public runtime_error {
    public:
        GameException(string message);
        
        virtual const char* what() const _GLIBCXX_USE_NOEXCEPT {
            return message.c_str();
        }

    protected:
        string message, stacktrace;
    };
    
    /**
     *  Neo: Whoa. Déjà vu.
     *  Trinity: What did you just say?
     *  Neo: Nothing. Just had a little déjà vu.
     *  Trinity: What did you see?
     *  Cypher: What happened?
     *  Neo: A black cat went past us, and then another that looked just like it.
     *  Trinity: How much like it? Was it the same cat?
     *  Neo: It might have been. I'm not sure.
     *  Morpheus: Switch! Apoc!
     *  Neo: What is it?
     *  Trinity: A déjà vu is usually a glitch in the Matrix. It happens when 
     *      they change something. 
     */
    class GameStateChanged : public GameException { 
    public:
        GameStateChanged(string message) : GameException(message) { }
    };
    
    /**
     * The target object has been removed from the world during an dialog.
     */
    class ObjectLost : public GameException {
    public:
        ObjectLost(string message = "Object has vanished in the meantime.") : GameException(message) { }
    };
    
    /**
     * I see you've equipped a nice creature, can I walk through and drink it?
     */
    class GameStateInvalid : public GameException {
    public:
        GameStateInvalid(string message) : GameException(message) { }
        
        const static string EquippedNonWearable, BackpackNotInventory, EquippedMoreThanOne;
    };

    
    /**
     * We tried hard to type-cast an object, but it wasn't possible
     * Indicates some logic error in the world.
     */
    class InvalidType : public GameStateInvalid {
    public:
        InvalidType(string message = "Object should have been of different type.") : GameStateInvalid(message) { }
    };
}

#endif

