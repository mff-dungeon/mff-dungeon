#ifndef SENTENCEJOINER_HPP
#define	SENTENCEJOINER_HPP

#include "../common.hpp"
#include "../Objects/Virtual/IDescriptable.hpp"
#include <ios>

namespace Dungeon {

    /**
     * Class that returns properly formatted sentence from parts.
     * 
     * Usage:
     *      SentenceJoiner rnd;
     *      rnd << "one" << "two" << "three"; 
     *   
     *      rnd.getSentence("There is %.", "There are %." ); 
     *                == "There are one, two and three."
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
            return count > 1 ? parts.str() + lastConjunction + last : last;
        }

        /**
         *  Use % to indicate place where to put the parts (or, fifth parameter)
         * @return formatted sentence
         */
        string getSentence(string zero, string one, string more = "", bool decapitalize = true, char placeholder = '%')
        {
            if (count == 0)
                return zero;
            
            string sentence = decapitalize ? Utils::decapitalize(getSentence()) : getSentence();
            if (count == 1 || more == "")
                return one.replace(one.find(placeholder), 1, sentence);
            return more.replace(more.find(placeholder), 1, sentence);
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
                if (count++ > 1) parts << conjunction;
                parts << last;
                last = value;
            }
            return *this;
        }

        /**
         * Because name of object is the most used part
         */
        SentenceJoiner& operator<<(ObjectPointer value)
        {
            if (value->instanceOf(IDescriptable))
                return *this << value.unsafeCast<IDescriptable>()->getName();
            else
                return *this  << value.getId();
        }
        
        SentenceJoiner& setConjunction(string all, string last) {
            conjunction = all;
            lastConjunction = last;
            return *this;
        }

        int size() {
            return count;
        }
    private:
        int count = 0;
        stringstream parts;
        string last = "";
        
        string conjunction = ", ";
        string lastConjunction = " and ";
    };
}

#endif

