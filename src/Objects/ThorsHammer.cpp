/* 
 * File:   ThorsHammer.cpp
 * Author: aearsis
 * 
 * Created on 5. srpna 2014, 16:24
 */

#include "ThorsHammer.hpp"
#include "../GameManager.hpp"
#include "../Actions/CallbackAction.hpp"
#include "../ActionDescriptor.hpp"

namespace Dungeon
{
	ThorsHammer::ThorsHammer() {
		this->setId("ThorsHammer");
	}
	
	ThorsHammer::~ThorsHammer() {
	}

	void ThorsHammer::getActions(ActionList* list, IObject* callee) {
		list->push_back(new CallbackAction("shutdown", "Shutdown's the server.",
				RegexMatcher::matcher("server shutdown"), 
				[] (ActionDescriptor* ad) {
					ad->getGM()->shutdown();
					ad->message = "OK. I will just finish the queue. Bye!";
				}));
	}
	
	void ThorsHammer::serialize(Archiver &stream) {
		
	}
	
    PERSISTENT_IMPLEMENTATION(ThorsHammer)
			
	
			
}
