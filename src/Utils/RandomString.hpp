#ifndef RANDOMSTRING_HPP
#define	RANDOMSTRING_HPP

#include "../common.hpp"
#include <ios>

namespace Dungeon {

    /**
     * Class that returns random string from a set.
     * 
     * Usage:
     *      RandomString rnd;
     *      rnd << "String 1" << endr
     *      rnd << "String 2" << endr;
     *   
     *      rnd.getRandom();
     * 
     * However, more interesting usage is with shortcuts:
     *      return RandomString::get()
     *          << "String 1" << endr
     *          << "String 2" << endr;
     * 
     * RS can be type casted into string. Type casting the singleton instance 
     * clears the list, otherwise it can be used multiple times.
     */
    class RandomString {
    public:
        RandomString();

        virtual ~RandomString()
        {
        }

        /** Special type to indicate EOM - @see endr*/
        class EndOfMessage {
        };

        /**
         * @return Random string
         */
        string getRandom();

        /**
         * @return Random string
         */
        operator string() {
            string r = getRandom();
            if (this == RandomString::instance)
                    messages.clear();
            return r;
        }

        /**
         * Finishes current message. @see endr
         */
        RandomString& operator<<(RandomString::EndOfMessage* (*endofmessage)());

        /**
         * Add a part to current message
         */
        template<typename T>
        RandomString& operator<<(T value)
        {
            current << value;
            return *this;
        }

        /**
         * Get singleton instance. It will be cleared after string typecast.
         * Provided only for convenience, this class can be instatiated and 
         * used more than once.
         */
        static RandomString& get();


    private:
        static RandomString* instance;

        vector<string> messages;
        stringstream current;
        default_random_engine random;
    };

    /**
     * Special value that breaks the stream and divides distinct messages.
     * Do not call it, it makes sense only in stream context of RandomString.
     * @return NULL
     */
    RandomString::EndOfMessage* endr();

}

#endif

