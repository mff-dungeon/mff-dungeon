#include "Table.hpp"

namespace Dungeon {
namespace Output {
 
	// Works perfectly for monospaced font and tabs of size 8.
	string Table::plainString() const {
		vector<size_t> colLengths(table[0].size());
		// Compute the size of columns
		for (auto& r : table) {
			if (r.size() > colLengths.size()) colLengths.resize(r.size());
			for (size_t i = 0; i < r.size(); i++) {
				size_t rlen = r[i].length();
				if (rlen > 0) rlen++; // Space after
				colLengths[i] = colLengths[i] > rlen ? colLengths[i] : rlen;
			}
		}
		for (size_t i = 0; i < colLengths.size(); i++)
			colLengths[i] = ((colLengths[i] + 7) / 8);

		stringstream str;
		for (auto& r : table) {
			for (size_t i = 0; i < r.size(); i++) {
				size_t rlen = r[i].length();
				if (rlen > 0) {
					rlen++;
					str << r[i] << " ";
				}	
				size_t tablen = colLengths[i] - rlen / 8;
				str << string(tablen, '\t');
			}
			str << "\n";
		}
		return str.str();
	}
	
	void Table::xhtml(Tag * parent) const {
		Tag * ta = new Tag(parent, "table");
		for (auto& r : table) {
			ta->addCData("\n\t");
			Tag * tr = new Tag(ta, "tr");
			for (auto& c : r) {
				new Tag(tr, "td", c + " ");
			}
		}
	}

}
}

