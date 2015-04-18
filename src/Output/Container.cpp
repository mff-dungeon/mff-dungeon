#include "Container.hpp"
#include "../Utils/SentenceJoiner.hpp"

namespace Dungeon {
namespace Output {

	string Container::plainString() const {
		stringstream ss;
		DisplayMode mode = Block;
		for (auto& ptr : contents) {
			if (mode == Line && ptr->displayMode == Block)
				ss << "\n";
			ss << ptr->plainString();
			mode = ptr->displayMode;
		}
		return ss.str();
	}

	string ListContainer::plainString() const {
		stringstream ss;
		for (auto& ptr : contents) {
			ss << " - " << ptr->plainString() << "\n";
		}
		return ss.str();
	}

	string SentenceContainer::plainString() const {
		SentenceJoiner sj;
		sj.setConjunction(" ");
		for (auto& ptr : contents) {
			sj << ptr->plainString();
		}
		return sj;
	}

	void Container::xhtml(Tag * parent) const {
		for (auto& ptr : contents) {
			ptr->xhtml(parent);
		}
	}

	void ListContainer::xhtml(Tag * parent) const {
		Tag * ul = new Tag(parent, "ul");
		for (auto& ptr : contents) {
			ul->addCData("\t");
			auto li = new Tag(ul, "li");
			ptr->xhtml(li);
			ul->addCData("\n");
		}
	}

	void SentenceContainer::xhtml(Tag * parent) const {
		Tag * p = new Tag(parent, "p");
		for (const ptr_t& ptr : contents) {
			ptr->xhtml(p);
			p->addCData(" ");
		}
	}

}
}

