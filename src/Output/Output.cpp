#include "Output.hpp"

namespace Dungeon {
namespace Output {

	string PlainString::plainString() const {
		return str;
	}

	string Newline::plainString() const {
		return "\n";
	}

	void PlainString::xhtml(Tag* parent) const {
		parent->addCData(str);
	}

	void FormattedString::xhtml(Tag* parent) const {
		new Tag(parent, tag, str);
	}

	void Newline::xhtml(Tag* parent) const {
		new Tag(parent, "br");
	}

}
}

