#include "Table.hpp"
#include "../Utils/SentenceJoiner.hpp"

namespace Dungeon {
namespace Output {

	/**
	 * There once was code which formated the table using tabs.
	 */
	string Table::plainString() const {
		stringstream ss;
		
		for (auto& r : table) {
			SentenceJoiner sj;
			sj.setConjunction(", ", ", ");
			for (auto& c : r) {
				sj << c->plainString();
			}
			ss << sj.getSentence() << endl;
		}
		return ss.str();
	}

	void Table::xhtml(Tag * parent) const {
		Tag * ta = new Tag(parent, "table");
		for (auto& r : table) {
			ta->addCData("\n\t");
			Tag * tr = new Tag(ta, "tr");
			for (auto& c : r) {
				Tag * td = new Tag(tr, "td");
				c->xhtml(td);
			}
		}
	}

}
}

