#include <string>
#include <sstream>
#include <sqlite3.h>
#include "common.hpp"

#ifndef DATABASEHANDLER_HPP
#define	DATABASEHANDLER_HPP

using namespace std;

/*
 * This class is supposed to handle all the database connection.
 * It is implemented as a singleton class.
 * WARNING: The class is implemented considering only single thread access.
 * Table design:
 *		CREATE TABLE objects (
 *		id TEXT NOT NULL,
 *		className TEXT NOT NULL,
 *		data BLOB NOT NULL
 * );
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
	};
}
#endif	/* DATABASEHANDLER_HPP */

