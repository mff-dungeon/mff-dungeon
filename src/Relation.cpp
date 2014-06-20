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

	string Relation::getWhere() {
		bool valid = false;
		stringstream query;
		query << "DELETE FROM relations WHERE ";
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
		query << ";";
		if(!valid) return "";
		else return query.str();
		
	}
}
