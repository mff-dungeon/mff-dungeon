#ifndef FUZZYSTRINGMATCHER_HPP
#define	FUZZYSTRINGMATCHER_HPP

#include <map>
#include <algorithm>
#include <exception>
#include <cstring>
#include "../common.hpp"

namespace Dungeon {

	/**
	 * Uses some kind of Damerau-Levenshtein distance to find best matching string.
	 *
	 * TODO add some treshold to filter out more good enough matches
	 */
	template <typename value_type>
	class FuzzyStringMatcher {
	public:
		FuzzyStringMatcher() { }
		FuzzyStringMatcher(int deleteCost, int insertCost, int substCost, int transCost) : deleteCost(deleteCost), insertCost(insertCost), substCost(substCost), transCost(transCost) { }
		FuzzyStringMatcher(const FuzzyStringMatcher& orig);
		virtual ~FuzzyStringMatcher() { }

		int WordMatch = 100;
                
                /**
                 * Bonus for matching all words
                 */
                int matchAllBonus = 10;

		/**
		 * Costs are taken as changes needle --> haystack
		 */
		int insertCost = 1,
				deleteCost = 3,
				substCost = 4,
				transCost = 1;

		/**
		 * Strings are marked same if this cost isn't exceeded
		 */
		int maxToleration = 5;

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

		template <typename value_type>
		class Uncertain : public GameException {
		public:
			Uncertain() : GameException("I'm not sure what you mean.") { }
			Uncertain(string msg) : GameException(msg) { }
			Uncertain(vector<value_type> possibleTargets) : GameException("Please explain it beter."), possibleTargets(possibleTargets) { }
			virtual ~Uncertain() { }

			vector<value_type> possibleTargets;
		};

		class NoCandidate : public GameException {
		public:
			NoCandidate() : GameException("I'm not sure you know what you mean.") { }
			NoCandidate(string msg) : GameException(msg) { }
			virtual ~NoCandidate() { }
		};
		;
		static bool matchTrueFalse(const string& text) {
			static FuzzyStringMatcher<bool> matcher;
			if (matcher.empty()) {
				matcher
						.add("1", true)
						.add("y", true)
						.add("ok", true)
						.add("yes", true)
						.add("yeah", true)
						.add("true", true)
						.add("of course", true)
						.add("0", false)
						.add("n", false)
						.add("no", false)
						.add("not", false)
						.add("nope", false)
						.add("false", false)
						.add("of course not", false);
			}
			return matcher.find(text);
		}

		static vector<string> tokenize(const string& str);
	};

	template<typename value_type>
	FuzzyStringMatcher<value_type>::FuzzyStringMatcher(const FuzzyStringMatcher& orig) : strMap(orig.strMap) { }

	template<typename value_type>
	FuzzyStringMatcher<value_type>& FuzzyStringMatcher<value_type>::add(string searchstring, value_type value) {
		if (searchstring.length() == 0) {
			LOGS(Warning) << "Adding empty string." << LOGF;
			return *this;
		}
		strMap.insert(pair<string, value_type>(searchstring,value));
		if (strMap.size() == 2) {
			strMap.insert(pair<string, value_type>(string("any"),value));
			strMap.insert(pair<string, value_type>(string("anything"),value));
		}
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
		size_t i = 0, sum = 0, lastmatch = 0, matched = 0;
		for (const string& nWord : needle) {
			for (; i < haystack.size(); i++) {
				int tol = std::min(nWord.length(), haystack[i].length());
				tol = std::min(maxToleration, tol);
				int cost = getWordDistance(nWord, haystack[i]);
				if (cost < tol) {
					sum += WordMatch - cost;
					lastmatch = i + 1;
                                        matched++;
					break; // next word from needle
				}
			}
			i = lastmatch; // this one is missing - let's try another
		}
		return sum + (matched == haystack.size()) * matchAllBonus;
	}

	template<typename value_type>
	value_type FuzzyStringMatcher<value_type>::find(const string& needle) {
		LOGS(Debug) << "Looking for '" << needle << "'" << LOGF;
		int max = 0;
		auto maxMatch = strMap.begin();
		bool uncertain = true;
		vector<value_type> possibleMatches;
		vector<string> nTok = StringMatcher::tokenize(needle);

		for (auto pair = strMap.begin(); pair != strMap.end(); pair++) {
			vector<string> hTok = StringMatcher::tokenize(pair->first);
			int d = getEqualness(nTok, hTok);
			LOGS(Debug) << " in " << pair->first << " -> " << d << " equalness" << LOGF;
			if (d > max) {
				max = d;
				maxMatch = pair;
				uncertain = false;
				possibleMatches.clear();
				possibleMatches.push_back(pair->second);
			} else if (d == max) {
				uncertain = uncertain || maxMatch->second != pair->second; // The same object with different names
				possibleMatches.push_back(pair->second);
			}
		}
		if (max == 0)
			throw StringMatcher::NoCandidate();

		if (uncertain)
			throw StringMatcher::Uncertain<value_type>(possibleMatches);

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

