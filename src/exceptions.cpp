#include "exceptions.hpp"

namespace Dungeon {

    const string GameStateInvalid::EquippedNonWearable = "You've managed to equip something that's not intended to be equipped.";
    const string GameStateInvalid::BackpackNotInventory = "Your backpack is something that can't act as a backpack.";
    const string GameStateInvalid::EquippedMoreThanOne = "You've managed to equip more than one item in one slot.";

	GameException::GameException(string message) : runtime_error(""), message(message) {
		// Possibly save the stacktrace
	}



}