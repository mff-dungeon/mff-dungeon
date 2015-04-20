#ifndef PROGRESSBAR_HPP
#define	PROGRESSBAR_HPP

#include "../common.hpp"
#include "Output.hpp"

namespace Dungeon {
namespace Output {

	class ProgressBar : public Base
	{
	public:
		ProgressBar(const int val, const int max, const int length, const string& fgColor = "#000000", const string& bgColor = "#FFFFFF")
            : fill(val * length / max), length(length), fgColor(fgColor), bgColor(bgColor) {}

        virtual string plainString() const;

        virtual void xhtml(Tag* parent) const;

	private:
        int fill, length;
        const string fgColor, bgColor;
	};
}
}

#endif

