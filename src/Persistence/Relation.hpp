#include <string>
#include "../common.hpp"

#ifndef RELATION_HPP
#define RELATION_HPP

namespace Dungeon {

	/**
	 * Commonly used relation types
	 */
	// Alive is in a room, doors are in the room
	#define R_INSIDE "inside"
	// Alive has backpack, backpack has potion
	#define R_INVENTORY "inventory"
	// Door targets somewhere
	#define R_TARGET "target"
	// Creature drops an item
	#define R_DROP "dropper"
	// Creature drops an item
	#define R_RECIPE "crafter-recipe"
	// Something requires a stat
	#define R_REQUIREMENT "stat-req"
	// Something has modular behavior
	#define R_BEHAVIOR "behavior"
	// Alive has gold
	#define R_RESOURCE(type) ( "resource-" + (string)(Resource::ResourceIdentifier[(int)type]) )
	// Alive explores stuff
	#define R_SEEN "seen"

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
	 *
	 * Also holds all common relations as static constants, for quick and easy access
	 */
	class Relation {
	public:
		objId mid, sid;
		string relation;

		enum Dir : bool {
			Slave = false,
			Master = true
		};

		/**
		 * Constructs a new relation. May be supplied with empty string
		 * considering them as wildcards (*) for the database queries
		 * @param mid objectId of the master object
		 * @param sid objectId of the slave object
		 * @param relation the identifier of the relation
		 */
		Relation(const objId& mid, const objId& sid, const string& relation) : mid(mid), sid(sid), relation(relation) { }

		/**
		 * Forms a select query matching the member variables of the object
		 * @return a string containing a database query
		 */
		string getSelectQuery() const;

		/**
		 * Forms a delete query matching the member variables of the object
		 * @return a string containing a database query
		 */
		string getDeleteQuery() const;

	private:
        /**
        * Private method for creating the common WHERE part of the query
        * @param query stream to insert query into
        * @return validity of the WHERE clause
        */
		bool addWhere(stringstream& query) const;
	};

	inline Relation::Dir operator!(const Relation::Dir dir) { return Relation::Dir(!(bool) dir); }
}

#endif
