#include "Relation.hpp"
#include <sstream>

namespace Dungeon {

	Relation::Relation(objId pid, objId sid, string pclass, string sclass, string relation) {
		this->pid = pid;
		this->sid = sid;
		this->pclass = pclass;
		this->sclass = sclass;
		this->relation = relation;
	}
	
	string Relation::getSelectQuery() {
		stringstream query;
		query << "SELECT * FROM relations ";
		bool result = this->addWhere(query);
		
		if(!result) return "";
		else {
			query << ";";
			return query.str();
		}
	}
	
	string Relation::getDeleteQuery() {
		stringstream query;
		query << "DELETE FROM relations ";
		bool result = this->addWhere(query);
		
		if(!result) return "";
		else {
			query << ";";
			return query.str();
		}
	}

	bool Relation::addWhere(stringstream& query) {
		bool valid = false;
		query << "WHERE ";
		if(pid != "0") {
			query << "pid = '" << pid << "'";
			valid = true;
		}
		if(sid != "0") {
		    if(valid) query << " AND ";
		    else valid = true;
			query << "sid = '" << sid << "'";
		}
		if(pclass != "0") {
			if(valid) query << " AND ";
			else valid = true;
			query << "pclass = '" << pclass << "'";
		}
		if(sclass != "0") {
			if(valid) query << " AND ";
			else valid = true;
			query << "sclass = '" << sclass << "'";
		}
		if(relation != "0") {
			if(valid) query << " AND ";
			else valid = true;
			query << "relation = '" << relation << "'";
		}
		return valid;	
	}
}
