#include "Crafter.hpp"

namespace Dungeon {
	
	Crafter::Crafter() {

	}
	
	Crafter::Crafter(objId id) : IDescriptable(id) {
		
	}

	Crafter::~Crafter() {
		
	}

	Crafter* Crafter::addRecipe(ObjectPointer recipe) {
		recipe.assertExists("The recipe doesn't exist").assertType<Recipe>("You can only add recipes. ");
		getGameManager()->createRelation(this, recipe, R_RECIPE);
		return this;
	}

	Crafter* Crafter::removeRecipe(ObjectPointer recipe) {
		recipe.assertExists("The recipe doesn't exist").assertType<Recipe>("You can only remove recipes. ")
			.assertRelation(R_RECIPE, recipe);
		getGameManager()->removeRelation(this, recipe, R_RECIPE);
		return this;
	}

	void Crafter::getActions(ActionList* list, ObjectPointer callee) {
		// Info action
		CraftAction* craftAct = new CraftAction;
		craftAct->addTarget(this);
		list->addAction(craftAct);
		
		// Recipes action
		CreateAction* createAct = new CreateAction;
		try {
			callee.assertType<Human>("Only humans can craft. ");
			int level = callee.unsafeCast<Human>()->getCraftingLevel();
			
			const ObjectMap& recipes = getRelations(Relation::Master, R_RECIPE);
			for(auto& recipe : recipes) {
				recipe.second.assertExists("Recipe somehow disappeared. ").assertType<Recipe>("This is not a recipe! ");
				if(recipe.second.unsafeCast<Recipe>()->getLevel() <= level) {
					createAct->addTarget(recipe.second);
				}
			}
			if(createAct->getTargets().size() > 0) {
				list->addAction(createAct);
			}
			else {
				delete createAct;
			}
		}
		catch (const std::out_of_range& e) {
			delete createAct;
		}
	}
	
	void CraftAction::explain(ActionDescriptor* ad) {
		*ad << "Use 'craft ...' to ask for a recipe list. ";
	}
	
	bool CraftAction::matchCommand(string command) {
		return RegexMatcher::match("craft .+", command);
	}
        
	void CraftAction::commit(ActionDescriptor* ad) {	
		commitOnBestTarget(ad, ad->in_msg.substr(6));
	}
        
	void CraftAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		target.assertExists("Crafter is not found. ").assertType<Crafter>("You can ask only a crafter for a craft list. ");
		if(!ad->getAlive()->isInstanceOf(Human::HumanClassName)) return;
		
		int level = ((Human*) ad->getAlive())->getCraftingLevel();
		try {
			const ObjectMap& recipes = target.unsafeCast<Crafter>()->getRelations(Relation::Master, R_RECIPE);
			if(recipes.empty()) return;
			for(auto& recipe : recipes) {
				recipe.second.assertExists("Recipe has disappeared. ").assertType<Recipe>("There is a non-recipe registered. ");
				Recipe* r = recipe.second.unsafeCast<Recipe>();
				if(level >= r->getLevel()) {
					*ad << r->getDescription();
				}
			}
		}
		catch (const std::out_of_range& e) {
			
		}
	}
	
	void CreateAction::explain(ActionDescriptor* ad) {
		*ad << "Use 'create ...' to use a recipe to create something. ";
	}
	
	bool CreateAction::matchCommand(string command) {
		return RegexMatcher::match("create .+", command);
	}
        
	void CreateAction::commit(ActionDescriptor* ad) {	
		commitOnBestTarget(ad, ad->in_msg.substr(7));
	}

	void CreateAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		target.assertExists("Recipe doesn't exists. ").assertType<Recipe>("You are trying to craft from a non-recipe. ");
		if(!ad->getAlive()->isInstanceOf(Human::HumanClassName)) return;
		
		Recipe* r = target.unsafeCast<Recipe>();
		int level = ((Human*) ad->getAlive())->getCraftingLevel();
		if(r->getLevel() > level) {
			*ad << "You cannot craft this item yet. ";
			LOGS("Crafter", Error) << "Somehow, a unsuitable item got to the create action. " << LOGF;
		}
		else {
			r->tryCraft(ad);
		}
	}

	PERSISTENT_IMPLEMENTATION(Crafter)
}