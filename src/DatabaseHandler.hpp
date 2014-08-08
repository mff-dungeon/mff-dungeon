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
		
		/*
		 * Error codes
		 */
		const int E_OK = 0;
		const int E_CONNECTION_ERROR = 1; // Unable to connect to database
		const int E_NO_ROW = 3; // No row was found
		const int E_UPDATE_ERROR = 5; // Unable to do an update query
		const int E_DELETE_ERROR = 6; // Unable to do a delete query
		const int E_TABLE_CREATE_ERROR = 10; // Unable to create a table
		const int E_TABLE_DROP_ERROR = 11; // Unable to delete a table
		const int E_COUNT_ERROR = 15; // Unable to count rows
		const int E_INVALID_QUERY = 20; // Invalid DB query
		
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

