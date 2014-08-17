#include "ThorsHammer.hpp"
#include "../GameManager.hpp"
#include "../Actions/CallbackAction.hpp"
#include "../ActionDescriptor.hpp"
#include "../ActionList.hpp"

namespace Dungeon
{
	ThorsHammer::ThorsHammer() {
		this->setId("ThorsHammer");
	}
	
	ThorsHammer::~ThorsHammer() {
	}

	void ThorsHammer::getActions(ActionList* list, IObject* callee) {
		list->addAction(new CallbackAction("shutdown", "server shutdown - Shutdown's the server.",
				RegexMatcher::matcher("server shutdown"), 
				[] (ActionDescriptor* ad) {
					ad->getGM()->shutdown();
					*ad << "OK. I will just finish the queue. Bye!";
				}));
	}
	
	void ThorsHammer::serialize(Archiver &stream) {
		
	}
	
    PERSISTENT_IMPLEMENTATION(ThorsHammer)
			
	
			
}
