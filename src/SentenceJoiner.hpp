#ifndef SENTENCEJOINER_HPP
#define	SENTENCEJOINER_HPP

#include "common.hpp"
#include <ios>

namespace Dungeon {

    /**
     * Class that returns properly formatted sentence from parts.
     * 
     * Usage:
     *      SentenceJoiner rnd;
     *      rnd << "one" << "two" << "three"; 
     *   
     *      rnd.getSentence("There is %. ", "There are %. " ); 
     *                == "There are one, two and three. "
     */
    class SentenceJoiner {
    public:
        SentenceJoiner() { }
        virtual ~SentenceJoiner() { }

        /**
         * @return formatted sentence
         */
        string getSentence()
        {
            return count > 1 ? parts.str() + " and " + last : last;
        }

        /**
         *  Use % to indicate place where to put the parts (or, fourth parameter)
         * @return formatted sentence
         */
        string getSentence(string zero, string one, string more = "", char placeholder = '%')
        {
            if (count == 0)
                return zero;
            if (count == 1 || more == "")
                return one.replace(one.find(placeholder), 1, getSentence());
            return more.replace(more.find(placeholder), 1, getSentence());
        }

        /**
         * @return formatted sentence
         */
        operator string() {
            return getSentence();
        }

        /**
         * Add a part to current message
         */
        SentenceJoiner& operator<<(string value)
        {
            if (value != "") {
                if (count++ > 1) parts << ", ";
                parts << last;
                last = value;
            }
            return *this;
        }

    private:
        int count = 0;
        stringstream parts;
        string last = "";
    };
}

#endif

