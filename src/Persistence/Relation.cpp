#include "Relation.hpp"
#include <sstream>

namespace Dungeon {

	string Relation::getSelectQuery() const {
		stringstream query;
		query << "SELECT * FROM relations ";
		bool result = this->addWhere(query);

		if(!result) return "";
		else {
			query << ";";
			return query.str();
		}
	}

	string Relation::getDeleteQuery() const {
		stringstream query;
		query << "DELETE FROM relations ";
		bool result = this->addWhere(query);

		if(!result) return "";
		else {
			query << ";";
			return query.str();
		}
	}

	bool Relation::addWhere(stringstream& query) const {
		bool valid = false;
		query << "WHERE ";
		if(mid != "0") {
			query << "mid = '" << mid << "'";
			valid = true;
		}
		if(sid != "0") {
		    if(valid) query << " AND ";
		    else valid = true;
			query << "sid = '" << sid << "'";
		}
		if(relation != "0") {
			if(valid) query << " AND ";
			else valid = true;
			query << "relation = '" << relation << "'";
		}
		return valid;
	}
}
