#include "Driver.hpp"
namespace Dungeon {

	Driver::Driver(ActionQueue* queue): queue(queue) {
        queue->registerDriver(this);
	}
    
    Driver::~Driver() {
        queue->unregisterDriver(this);
    }
    
    void Driver::processDescriptor(ActionDescriptor* descriptor) {
        LOG("Driver") << "Action descriptor arrived: '" << descriptor->message << "'." << LOGF;
    }

}

