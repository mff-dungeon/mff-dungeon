#include <string>
#include "common.hpp"

#ifndef RELATION_HPP
#define RELATION_HPP

/**
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

		/**
		 * Constructs a new relation. May be supplied with empty string 
		 * considering them as wildcards (*) for the database queries
         * @param pid objectId of the parent object
         * @param sid objectId of the son object
         * @param pclass className of the parent object
         * @param sclass className of the son object
         * @param relation the identifier of the relation
         */
		Relation(objId pid, objId sid, string pclass,
			string sclass, string relation);
		
		/**
		 * Forms a select query matching the member variables of the object
         * @return a string containing a database query
         */
		string getSelectQuery();
		
		/**
		 * Forms a delete query matching the member variables of the object
         * @return a string containing a database query
         */
		string getDeleteQuery();
	private:
		/**
		 * Private method for creating the common WHERE part of the query
         * @param query stream to insert query into
         * @return validity of the WHERE clause
         */
		bool addWhere(stringstream& query);
	};
}

#endif
