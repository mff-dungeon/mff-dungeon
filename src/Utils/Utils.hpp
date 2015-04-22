#ifndef UTILS_HPP
#define	UTILS_HPP

/*
 *  Random number generation
 */
#include <random>
#define rand_init(var, dist, min, max) std::default_random_engine var(Dungeon::hw_random()); std::uniform_int_distribution<int> dist(min, max);
#define rand_next(var, dist) ( dist(var) )
#define RAND_ALNUM "qwertyuiopasdfghjklzxcvbnm0123456789"
#define RAND_ALNUM_LEN 36
#define RANDID Dungeon::Utils::getRandomString(6)

/*
 *  Helper functions for accessing parts of id
 */
#define objId_getType(id) ( id.substr(0, id.rfind("/")) )
#define objId_getIdentifier(id) ( id.substr(id.rfind("/") + 1) )

#include "../common.hpp"
#include <stdexcept>

namespace Dungeon {

    static random_device hw_random;
    class ActionDescriptor;

    class Utils {
    public:


        static string getRandomString(int len)
        {
            char ret [len + 1];
            ret[len] = 0;
            static rand_init(r, d, 0, RAND_ALNUM_LEN - 1);
            for (int i = 0; i < len; ++i) ret[i] = RAND_ALNUM[rand_next(r, d)];
            return ret;
        }

        static int getRandomInt(int min, int max)
        {
            static rand_init(r, d, min, max);
            return rand_next(r, d);
        }

        static string weightStr(const int weight)
        {
            char buf [50];
            if (weight > 1000000)
                sprintf(buf, "%.2f t", weight / 1016046.9088);
            else if (weight > 1000)
                sprintf(buf, "%.2f lb", weight / 453.59237);
            else
                sprintf(buf, "%.2f oz", weight / 28.349523125);
            return string(buf);
        }

        // courtesy of http://stackoverflow.com/questions/1798112/removing-leading-and-trailing-spaces-from-a-string

        static inline std::string ltrim(std::string s, const char* t = " \t\n\r\f")
        {
            s.erase(0, s.find_first_not_of(t));
            return s;
        }

        static inline std::string rtrim(std::string s, const char* t = " \t\n\r\f")
        {
            s.erase(s.find_last_not_of(t) + 1);
            return s;
        }

        static inline std::string trim(const std::string& s, const char* t = " \t\n\r\f")
        {
            return ltrim(rtrim(s, t), t);
        }

        static inline std::string& capitalize(std::string&& s, bool all = false)
        {
            bool cap = true;

            for (size_t i = 0; i <= s.length(); i++) {
                if (isalpha(s[i]) && cap) {
                    s[i] = toupper(s[i]);
                    cap = false;

                    if (!all) break;
                } else if (isspace(s[i])) {
                    cap = true;
                }
            }

            return s;
        }

        static inline std::string decapitalize(std::string&& s)
        {
            for (size_t i = 0; i <= s.length(); i++) {
                if (isalpha(s[i])) {
                    s[i] = tolower(s[i]);
                }
            }
            return s;
        }

        static inline std::string decapitalize(const std::string& cs)
        {
            std::string s(cs);
            for (size_t i = 0; i <= s.length(); i++) {
                if (isalpha(s[i])) {
                    s[i] = tolower(s[i]);
                }
            }
            return s;
        }

        /**
         * Gets current time in specified format
         * @param format datetime formatting used in strftime method
         * @return a string with the current time
         */
        static std::string currentTime(string format) {
                std::time_t t = std::time(NULL);
                char mbstr[100];
                std::strftime(mbstr, sizeof (mbstr), format.c_str(), std::localtime(&t));

                return std::string(mbstr);
        }
        
        template<char placeholder = '%', typename T, typename... Args>
        static std::string formatMessage(const string& format, T&& val, Args&&... values) {
            string copy = formatMessage<placeholder>(format, forward<Args>(values)...);
            size_t pos = copy.find(placeholder);
            if (pos == string::npos)
                throw std::invalid_argument("Not enough placeholders in '" + format + "'");
            return copy.replace(pos, 1, val);
        }
        
        template<char placeholder = '%'>
        static std::string formatMessage(const string& format) {
            return format; // Copying for recursive purposes
        }
    };
}

#endif

