#include "Recipe.hpp"

namespace Dungeon {

	Recipe::Recipe() {

	}
	
	Recipe::Recipe(objId id) : IDescriptable(id) {

	}

	Recipe::~Recipe() {

	}

	int Recipe::getLevelRequired() const {
		return level;
	}

	Recipe* Recipe::setLevelRequired(int level) {
		this->level = level;
		return this;
	}

	string Recipe::getDescription() const {
		return getName() + ": " + "blabla requirements";
	}

	int Recipe::getResource(int type) {
		return resources[type];
	}

	Recipe* Recipe::setResource(int resource, int type) {
		this->resources[type] = resource;
		return this;
	}

	Recipe* Recipe::setResourceList(int* resources) {
		this->resources = resources;
		return this;
	}

	void Recipe::tryCraft(ActionDescriptor* ad) {
		// TODO check resources, create item, bla bla
		return;
	}

	void Recipe::registerProperties(IPropertyStorage& storage) {
		// TODO add all resources
	}





	
	PERSISTENT_IMPLEMENTATION(Recipe)
}


