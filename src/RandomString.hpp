#ifndef RANDOMSTRING_HPP
#define	RANDOMSTRING_HPP

#include "common.hpp"
#include <ios>

namespace Dungeon {

    class RandomString {
    public:
        RandomString();
        virtual ~RandomString() {}
        
        class EndOfMessage {};
        
        string getRandom();
        operator string ();
        
        RandomString& operator<<(RandomString::EndOfMessage* (*endofmessage)());
        
        template<typename T>
        RandomString& operator<<(T value) {
            current << value;
            return *this;
        }
        
        static RandomString& get();
        
    private:
        static RandomString* instance;
        
        vector<string> messages;
        stringstream current;
        default_random_engine random;
    };
    
    RandomString::EndOfMessage* endr();
    
}

#endif

