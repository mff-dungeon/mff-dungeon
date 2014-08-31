#ifndef FUZZYSTRINGMATCHER_HPP
#define	FUZZYSTRINGMATCHER_HPP

#include <map>
#include <algorithm>
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
        
        /**
         * Costs are taken as changes needle --> haystack
         */
        int insertCost = 1,
            deleteCost = 5,
            substCost = 5,
            transCost = 1;
        
        virtual FuzzyStringMatcher<value_type>& add(string searchstring, value_type value);
        void clear() {
            strMap.clear();
        }
        
        /**
         * TODO Get substCost from distance on QWERTY
         */
        virtual int getSubstCost(const char& a, const char& b) const;
        
        virtual int getDifference(const string& needle, const string& haystack) const;
        
        virtual value_type find(const string& needle);
        
        class Uncertain : public runtime_error { };
        class NoCandidate : public runtime_error { };
    private:
        map<string, value_type> strMap;
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
    int FuzzyStringMatcher<value_type>::getDifference(const string& needle, const string& haystack) const {
        const int nlen = needle.length(),
                  hlen = haystack.length();
        if (hlen == 0) return -1;
        vector<int> row1(hlen + 1, 0);
        vector<int> row2(hlen + 1, 0);
        for (int i = 0; i < hlen + 1; ++i)
            row2[i] = insertCost * i;
        
        for (int i = 0; i < nlen; ++i) {
                vector<int> row3(1, deleteCost * i + 1);
                for (int j = 0; j < hlen; ++j) {
                        const int scost = row2[j] + (needle[i] != haystack[j] ? getSubstCost(needle[i], haystack[j]) : 0);
                        const int icost = row2[j + 1] + deleteCost;
                        const int dcost = row3[j] + insertCost;
                        int tcost = dcost + 1000;

                        if (i > 1 && j > 1 && needle[i - 1] == haystack[j] && needle[i] == haystack[j - 1]) {
                                tcost = row1[j - 1] + transCost;
                        }
                        int min = std::min(std::min(scost, icost), std::min(tcost, dcost));
                        row3.push_back(min);
                }
                row1.swap(row2);
                row2.swap(row3);
        }
        return row2.back();
    }

    template<typename value_type>
    value_type FuzzyStringMatcher<value_type>::find(const string& needle) {
        LOGS("FuzzyMatcher", Verbose) << "Looking for " << needle << LOGF;
        int min = -1;
        value_type val = strMap.begin()->second;
        for (auto& pair : strMap) {
                int d = getDifference(needle, pair.first);
                LOGS("FuzzyMatcher", Verbose) << " in " << pair.first << " -> " << d <<  "difference" << LOGF;
                if (min == -1 || d < min) {
                        min = d;
                        val = pair.second;
                }
        }
        return val;
    }

    template<typename value_type>
    int FuzzyStringMatcher<value_type>::getSubstCost(const char& a, const char& b) const {
        return substCost;
    }
}

#endif

