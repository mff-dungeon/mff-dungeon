#include "ProgressBar.hpp"

namespace Dungeon {
namespace Output {
	
	string ProgressBar::plainString() const {
		return string(fill, '#') + string(length - fill, '-');
	}
	
	void ProgressBar::xhtml(Tag* parent) const {
		Tag* fTag = new Tag(parent, "span");
		Tag* eTag = new Tag(parent, "span");
		fTag->addAttribute("style", "font-family: monospace; background-color: " + fgColor + "; color: " + fgColor);
		eTag->addAttribute("style", "font-family: monospace; background-color: " + bgColor + "; color: " + bgColor);
		fTag->setCData(string(fill, '#'));
		eTag->setCData(string(length - fill, '-'));
	}

}
}
