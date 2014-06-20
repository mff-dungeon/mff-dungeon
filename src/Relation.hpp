#include <string>
#include "common.hpp"

#ifndef RELATION_HPP
#define RELATION_HPP

/*
 * Object representing a relation row in the DB.
 *
 * It is used mainly for easier and more comprehensible transfer of relations
 * across objects, useful for DB communication methods
 *
 * It has a public access to its members(can be remade later).
 *
 * Also implements a getWhere() method returning a WHERE clause according to 
 * set members (unset member has a value of "0"), or empty string if a WHERE 
 * clause can't be constructed.
 */
namespace Dungeon {

	class Relation {
	public:
		objId pid, sid;
		string pclass, sclass, relation;

		Relation(objId pid, objId sid, string pclass,
			string sclass, string relation);
		string getWhere();
	};
}

#endif
