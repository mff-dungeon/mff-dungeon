#include "Crafter.hpp"
#include "../Utils/RegexMatcher.hpp"
#include "../Actions/UseAction.hpp"
#include "../Game/ActionDescriptor.hpp"
#include "../Game/GameManager.hpp"
#include "Virtual/Recipe.hpp"
#include "Human.hpp"

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

	void Crafter::getActions(ActionList* list, ObjectPointer callee) {
		// Info action
		list->addAction(new CraftAction)
				->addTarget(this)
				->useActionFor(this, list);

		// Recipes action
		CreateAction* createAct = new CreateAction;
		try {
			callee.assertType<Human>("Only humans can craft.");

			const ObjectMap& recipes = getRelations(Relation::Master, R_RECIPE);
			for (auto& recipe : recipes) {
				recipe.second.assertExists("Recipe somehow disappeared.").assertType<Recipe>("This is not a recipe! ");
				if (recipe.second.unsafeCast<Recipe>()->checkStatReqs(callee)) {
					createAct->addTarget(recipe.second);
				}
			}
			if (createAct->getTargets().size() > 0) {
				list->addAction(createAct);
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
			for (auto& recipe : recipes) {
				recipe.second.assertExists("Recipe has disappeared.").assertType<Recipe>("There is a non-recipe registered.");
				Recipe* r = recipe.second.unsafeCast<Recipe>();
				if (r->checkStatReqs(ad->getCaller())) {
					*ad << r->getDescription() << eos;
					found++;
				}
			}
			if (!found) *ad << "You cannot craft anything here yet. Try to raise your stats." << eos;
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
			LOGS("Crafter", Error) << "Somehow, an unsuitable item got to the create action." << LOGF;
		} else {
			r->tryCraft(ad);
		}
	}

	PERSISTENT_IMPLEMENTATION(Crafter)
}