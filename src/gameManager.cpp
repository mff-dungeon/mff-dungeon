#include "common.hpp"
#include "splayTree.hpp"

namespace Dungeon {
	class GameManager {
		private:
			SplayTree objects;

		public:
			IObject* getObject(objId id);
	};
}
