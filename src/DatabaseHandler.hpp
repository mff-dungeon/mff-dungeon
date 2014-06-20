#include <string>
#include <sstream>
#include <sqlite3.h>
#include "common.hpp"
#include "Relation.hpp"

#ifndef DATABASEHANDLER_HPP
#define	DATABASEHANDLER_HPP

using namespace std;

/*
 * This class is supposed to handle all the database connection.
 * It is implemented as a singleton class.
 * WARNING: The class is implemented considering only single thread access.
 */

namespace Dungeon {
	
	class DatabaseHandler {
	private:
		sqlite3* dbConnection;
		sqlite3_stmt* dbStatement;
		int sqlCode;
		
		DatabaseHandler() { }		// Private constructor
		DatabaseHandler(const DatabaseHandler&);
		DatabaseHandler& operator=(const DatabaseHandler&);
		~DatabaseHandler() {};
	public:
		static DatabaseHandler& getInstance();
		int saveObject(objId oid, string cName, string data);
		int loadObject(objId oid, string& cName, stringstream& sData);
		int deleteObject(objId oid);
		int initDatabase(); // creates table structure
		int dropDatabase(); // leaves empty database

		/*
		 * Method dealing with object relations
		 * getRelations returns a vector of objects, that are in a specific relation
		 * addRelation adds a new relation
		 * deleteRelation deletes an existing relation(s)
		 */
		int getRelations(vector<objId>& result, Relation* rel, bool parent=false);
		int addRelation(Relation* rel);
		int deleteRelation(Relation* rel);
	};
}
#endif

