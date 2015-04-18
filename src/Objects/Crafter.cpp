#include "Crafter.hpp"
#include "../Utils/RegexMatcher.hpp"
#include "../Actions/UseAction.hpp"
#include "../Game/ActionDescriptor.hpp"
#include "../Game/GameManager.hpp"
#include "Virtual/Recipe.hpp"
#include "Human.hpp"
#include "Inventory.hpp"
#include "../Output/Table.hpp"

namespace Dungeon {

	Crafter* Crafter::addRecipe(ObjectPointer recipe) {
		recipe.assertExists("The recipe doesn't exist").assertType<Recipe>("You can only add recipes.");
		getGameManager()->createRelation(this, recipe, R_RECIPE);
		return this;
	}

	Crafter* Crafter::removeRecipe(ObjectPointer recipe) {
		recipe.assertExists("The recipe doesn't exist").assertType<Recipe>("You can only remove recipes.")
				.assertRelation(R_RECIPE, recipe);
		getGameManager()->removeRelation(this, recipe, R_RECIPE);
		return this;
	}

	void Crafter::getActions(ActionDescriptor* ad) {
		// Info action
		auto& list = ad->getActionList();
		list.addAction(new CraftAction)
				->addTarget(this)
				->useActionFor(this, ad);

		// Recipes action
		CreateAction* createAct = new CreateAction;
		try {
			const ObjectMap& recipes = getRelations(Relation::Master, R_RECIPE);
			for (auto& recipe : recipes) {
				recipe.second.assertExists("Recipe somehow disappeared.").assertType<Recipe>("This is not a recipe! ");
				if (recipe.second.unsafeCast<Recipe>()->checkStatReqs(ad->getCaller())) {
					createAct->addTarget(recipe.second);
				}
			}

			if (createAct->getTargets().size() > 0) {
				list.addAction(createAct);
			} else {
				delete createAct;
			}
		} catch (const std::out_of_range& e) {
			delete createAct;
		}
	}

	void CraftAction::explain(ActionDescriptor* ad) {
		*ad << "Use 'craft ...' to ask for a recipe list." << eos;
	}

	bool CraftAction::match(const string& command, ActionDescriptor* ad) {
		if (RegexMatcher::match("craft .+", command)) {
			selectBestTarget(command.substr(6), ad);
			return true;
		}
		return false;
	}

	void CraftAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		target.assertExists("Crafter is not found.").assertType<Crafter>("You can ask only a crafter for a craft list.");

		try {
			const ObjectMap& recipes = target.unsafeCast<Crafter>()->getRelations(Relation::Master, R_RECIPE);
			if (recipes.empty()) return;
			int found = 0;
			auto table = Output::Table::create();
			for (auto& recipe : recipes) {
				recipe.second.assertExists("Recipe has disappeared.").assertType<Recipe>("There is a non-recipe registered.");
				Recipe* r = recipe.second.unsafeCast<Recipe>();
				Human* human = ad->getCaller();
				Inventory* backpack = human->getBackpack().safeCast<Inventory>();
				if (r->checkStatReqs(human)) {
					if (r->hasEnoughMaterial(human)) {
						*table << Output::FormattedString::create("b", r->getName());
					} else {
						*table << r->getName();
					}
					for (int i = Resource::ResourceType::BEGIN; i != Resource::ResourceType::END; i++) {
						if (r->getResource(i) > 0)
							if (backpack->getResourceQuantity((Resource::ResourceType) i) >= r->getResource(i))
								*table << Output::FormattedString::create("b", to_string(r->getResource(i)) + " " + Resource::ResourceName[i]);
							else
								*table << to_string(r->getResource(i)) + " " + Resource::ResourceName[i];
						else 
							*table << "";
					}
					table->finishRow();
					found++;
				}
			}
			if (found) { 
				auto& out = ad->getOutputContainer();
				out << "You can create following items using these resources:";
				out << move(table);
			} else {
				*ad << "You cannot craft anything here yet. Try to raise your stats." << eos;
			}
		} catch (const std::out_of_range& e) {

		}
	}

	void CreateAction::explain(ActionDescriptor* ad) {
		*ad << "Use 'create ...' to use a recipe to create something." << eos;
	}

	bool CreateAction::match(const string& command, ActionDescriptor* ad) {
		if (RegexMatcher::match("create .+", command)) {
			selectBestTarget(command.substr(7), ad);
			return true;
		}
		return false;
	}

	void CreateAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		target.assertExists("Recipe doesn't exists.").assertType<Recipe>("You are trying to craft from a non-recipe.");

		Recipe* r = target.unsafeCast<Recipe>();
		if (!r->checkStatReqs(ad->getCaller(), ad)) {
			*ad << "You cannot craft this item yet." << eos;
			LOGS(Error) << "Somehow, an unsuitable item got to the create action." << LOGF;
		} else {
			r->tryCraft(ad);
		}
	}

	PERSISTENT_IMPLEMENTATION(Crafter)
}