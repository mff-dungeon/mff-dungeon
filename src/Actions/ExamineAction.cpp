#include "ExamineAction.hpp"

namespace Dungeon {

	bool ExamineEction::match(const string& command, ActionDescriptor* ad) {
		smatch matches;
		if (RegexMatcher::match("(investigate|examine|explore|look( closer)?( +(in|to|into|on|onto|at))?) +(.+)", command, matches)) {
			selectBestTarget(matches[5], ad);
			return true;
		}
		return false;
	}

	void ExamineEction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		target.assertType<IDescriptable>("Examining non-examinable object.")
				.unsafeCast<IDescriptable>()->examine(ad);
	}

	void ExamineEction::explain(ActionDescriptor* ad) {
		*ad << "examine ... - look closer on items you have or see." << eos;
	}

}