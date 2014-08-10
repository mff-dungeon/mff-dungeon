#include "Driver.hpp"
namespace Dungeon {

	Driver::Driver(ActionQueue* queue): queue(queue) {
        queue->registerDriver(this);
	}
    
    Driver::~Driver() {
        queue->unregisterDriver(this);
    }
    
    void Driver::processDescriptor(ActionDescriptor* descriptor) {
		if (typeid(*descriptor) == typeid(TextActionDescriptor))
			LOG("Driver") << "Action descriptor arrived: '" << ((TextActionDescriptor*)descriptor)->in_msg << "'." << LOGF;
        else
			LOG("Driver") << "Action descriptor arrived." << LOGF;
    }

}

