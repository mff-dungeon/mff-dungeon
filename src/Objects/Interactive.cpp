#include "Interactive.hpp"
#include "../Game/ActionDescriptor.hpp"
#include "../Game/GameManager.hpp"
#include "Behavior/BasicInteraction.hpp"
#include <string>

using namespace std;

namespace Dungeon {
	
	void Interactive::attachInteraction(const string& verb, const string& explanation, const string& content) {
		LOGS("Interactive", Verbose) << "Attaching interactive behavior on " << getId() << ",verb " << verb << LOGF;
		BasicInteraction* interaction = new BasicInteraction("trap/interaction/" + getId() + "/" + RANDID);
		interaction->setContent(content)
			->setVerb(verb)
			->setExplanation(explanation);
		getGameManager()->insertObject(interaction);

		getGameManager()->createRelation(this, interaction, R_BEHAVIOR);
	}

	PERSISTENT_IMPLEMENTATION(Interactive)

}

