#ifndef FUZZYSTRINGMATCHER_HPP
#define	FUZZYSTRINGMATCHER_HPP

#include <map>
#include <algorithm>
#include <exception>
#include <cstring>
#include "common.hpp"

namespace Dungeon {
    
    /**
     * Uses some kind of Damerau-Levenshtein distance to find best matching string.
     * 
     * TODO add some treshold to filter out more good enough matches
     */
    template <typename value_type>
    class FuzzyStringMatcher {
    public:
        FuzzyStringMatcher() {}
        FuzzyStringMatcher(int deleteCost, int insertCost, int substCost, int transCost) : deleteCost(deleteCost), insertCost(insertCost), substCost(substCost), transCost(transCost) {}
        FuzzyStringMatcher(const FuzzyStringMatcher& orig);
        virtual ~FuzzyStringMatcher() {}
        
        const static int WordMatch = 100;
        
        /**
         * Costs are taken as changes needle --> haystack
         */
        int insertCost = 1,
            deleteCost = 3,
            substCost = 4,
            transCost = 1;
        
        /**
         * Strings are marked same if this cost insn't exceeded
         */
        const static int toleration = 5;
        
        /**
         * TODO Get substCost from distance on QWERTY
         */
        virtual int getSubstCost(const char& a, const char& b) const;
        virtual int getWordDistance(const string& needle, const string& haystack) const;
        
        virtual int getEqualness(const vector<string>& needle, const vector<string>& haystack) const;
       
        void clear() {
            strMap.clear();
        }
        
        bool empty() {
            return strMap.empty();
        }
        
        virtual value_type find(const string& needle);
        virtual FuzzyStringMatcher<value_type>& add(string searchstring, value_type value);
        
    private:
        map<string, value_type> strMap;
    };
    
    class StringMatcher {
    public:
        class Uncertain : public runtime_error {
        public:
            Uncertain() : runtime_error("I'm not sure what you mean.") {}
            virtual ~Uncertain() {}                
        };
        class NoCandidate : public runtime_error {
        public:
            NoCandidate() : runtime_error("I'm not sure you know what you mean.") {}
            virtual ~NoCandidate() {}                
        };;
        
        static bool matchTrueFalse(const string& text) {
            static FuzzyStringMatcher<bool> matcher;
            if (matcher.empty()) {
                matcher.add("true", true)
                    .add("yes", true)
                    .add("yeah", true)
                    .add("ok", true)
                    .add("of course", true)
                    .add("1", true)
                    .add("no", false)
                    .add("false", false)
                    .add("nope", false)
                    .add("not", false)
                    .add("of course not", false)
                    .add("0", false);
            }
            
            return matcher.find(text);
        }
        
        static vector<string> tokenize(const string& str);
    };
    
    
    template<typename value_type>
    FuzzyStringMatcher<value_type>::FuzzyStringMatcher(const FuzzyStringMatcher& orig) : strMap(orig.strMap) {
    }

    template<typename value_type>
    FuzzyStringMatcher<value_type>& FuzzyStringMatcher<value_type>::add(string searchstring, value_type value) {
        strMap[searchstring] = value;
        return *this;
    }

    template<typename value_type>
    int FuzzyStringMatcher<value_type>::getWordDistance(const string& needle, const string& haystack) const {
        const int nlen = needle.length(),
                  hlen = haystack.length();
        if (hlen == 0) return -1;
        
        // Last 2 rows are remembered
        vector<int> row1(hlen + 1, 0);
        vector<int> row2(hlen + 1, 0);
        for (int i = 0; i < hlen + 1; ++i)
            row2[i] = insertCost * i;
        
        for (int i = 0; i < nlen; ++i) {
            // vector<int> row3(1, deleteCost * i + 1);
            vector<int> row3(1, 2); // Prefix is for fixed cost
            for (int j = 0; j < hlen; ++j) {
                // Calculate cost for substitution, insert and delete
                const int scost = row2[j] + (needle[i] != haystack[j] ? getSubstCost(needle[i], haystack[j]) : 0);
                const int icost = row2[j + 1] + deleteCost;
                const int dcost = row3[j] + insertCost;

                // And for transposition
                int tcost = dcost + 1000;
                if (i > 1 && j > 1 && needle[i - 1] == haystack[j] && needle[i] == haystack[j - 1]) {
                        tcost = row1[j - 1] + transCost;
                }
                
                // Choose the best
                int min = std::min(std::min(scost, icost), std::min(tcost, dcost));
                row3.push_back(min);
            }
            // Remember last two rows
            row1.swap(row2);
            row2.swap(row3);
        }
        return row2.back();
    }
    
    template<typename value_type>
    int FuzzyStringMatcher<value_type>::getEqualness(const vector<string>& needle, const vector<string>& haystack) const {
        int i = 0, sum = 0, lastmatch = 0;
        for (const string& nWord : needle) {
            for (; i < haystack.size(); i++) {
                int cost = getWordDistance(nWord, haystack[i]);
                if (cost < std::min(nWord.length(), haystack[i].length())) {
                    sum += WordMatch - cost;
                    lastmatch = i + 1;
                    break; // next word from needle
                }
            }
            i = lastmatch; // this one is missing - let's try another
        }
        return sum;
    }

    template<typename value_type>
    value_type FuzzyStringMatcher<value_type>::find(const string& needle) {
        LOGS("FuzzyMatcher", Verbose) << "Looking for " << needle << LOGF;
        int max = 0;
        auto maxMatch = strMap.begin();
        bool uncertain = true;
        vector<string> nTok = StringMatcher::tokenize(needle);
        
        for (auto pair = strMap.begin(); pair != strMap.end(); pair++) {
            vector<string> hTok = StringMatcher::tokenize(pair->first);
            int d = getEqualness(nTok, hTok);
            LOGS("FuzzyMatcher", Verbose) << " in " << pair->first << " -> " << d <<  " equalness" << LOGF;
            if (d > max) {
                max = d;
                maxMatch = pair;
                uncertain = false;
            } else if (d == max) {
                uncertain = true;
            }
        }
        if (max == 0)
            throw StringMatcher::NoCandidate();
        
        // FIXME: if uncommented, everything is uncertain
        /*if (uncertain)
            throw StringMatcher::Uncertain();*/
        
        return maxMatch->second;
    }

    template<typename value_type>
    int FuzzyStringMatcher<value_type>::getSubstCost(const char& a, const char& b) const {
        if (::tolower(a) == ::tolower(b))
            return 0;
        return substCost;
    }
}

#endif

