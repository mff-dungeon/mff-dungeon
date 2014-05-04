#include <cstring>
#include <string>
#include <cstdlib>

#include "DatabaseHandler.hpp"
#include "common.hpp"

namespace Dungeon {

	// get Singleton Instance
	DatabaseHandler& DatabaseHandler::getInstance() {
		static DatabaseHandler instance;
		return instance;
	}

	// Saves object from stringstream into DB, returns 0 if successful
	int DatabaseHandler::saveObject(objId oid, string cName, string cData) {
		sqlCode = sqlite3_open(DB_NAME, &dbConnection);
		if(sqlCode != SQLITE_OK) { 
			sqlite3_close(dbConnection);
			return 1; //DatabaseHandler error: Error when opening database
		}
		
		/*
		 Count, to find out if we should delete or update
		 */
		string scount = "SELECT COUNT(*) FROM objects WHERE id = " + oid + ";";
		const char *countstate = scount.c_str();
		sqlite3_prepare(dbConnection, countstate, 200, &dbStatement, 0);
		sqlCode = sqlite3_step(dbStatement);
		int count = 0;
		if(sqlCode == SQLITE_ROW) {
			count = sqlite3_column_int(dbStatement, 0);
		}
		else {
			sqlite3_finalize(dbStatement);
			sqlite3_close(dbConnection);
			return 3; //DatabaseHandler error: Error when counting database
		}
		if(!count) {
			const char *statement = "INSERT INTO objects (id, className, data) VALUES (?, ?, ?);";
			sqlite3_prepare_v2(dbConnection, statement, (int)strlen(statement), &dbStatement, 0);
		}
		else {
			const char *statement = "UPDATE objects SET id = ?, className = ?, data = ? WHERE id = ?;";
			sqlite3_prepare_v2(dbConnection, statement, (int)strlen(statement), &dbStatement, 0);
		}

		sqlite3_bind_text(dbStatement, 1, oid.c_str(), strlen(oid.c_str()), 0);
		sqlite3_bind_text(dbStatement, 2, cName.c_str(), strlen(cName.c_str()), 0);
		sqlite3_bind_blob(dbStatement, 3, cData.c_str(), cData.size(), 0);
		if(count) sqlite3_bind_text(dbStatement, 4, oid.c_str(), strlen(oid.c_str()), 0);

		sqlCode = sqlite3_step(dbStatement);
		if(sqlCode != SQLITE_DONE) { 
			sqlite3_finalize(dbStatement);
			sqlite3_close(dbConnection);
			return 2; //DatabaseHandler error: Error when updating database
		}
		sqlite3_finalize(dbStatement);
		sqlite3_close(dbConnection);

		return 0;
	}

	// Loading object into a stringstream, returns 0 if loaded successfuly
	int DatabaseHandler::loadObject(objId oid, string& cName, stringstream& sData) {
		sqlCode = sqlite3_open(DB_NAME, &dbConnection);
		if(sqlCode != SQLITE_OK) { 
			sqlite3_close(dbConnection);
			return 1; //DatabaseHandler error: Error when opening database
		}

		string statestring = "SELECT * FROM objects WHERE id = " + oid + ";";
		const char *state = statestring.c_str();
		sqlite3_prepare(dbConnection, state, 200, &dbStatement, 0);
		sqlCode = sqlite3_step(dbStatement);
		if(sqlCode == SQLITE_ROW) {
			cName = std::string((char *) sqlite3_column_text(dbStatement, 1));
			int size = sqlite3_column_bytes(dbStatement, 2);
			unsigned char* datac = new unsigned char[size];
			const void *dataptr = sqlite3_column_blob(dbStatement, 2);
			/* 
			 * Trick - using std::string(datac) results in a failure because of
			 * null bytes (e.g. int i = 1 === 0x00000001, which fails to copy
			 * to string, therefore copying the field byte after byte.
			 */
			memcpy(datac, dataptr, size);
			for(int i=0; i<size; i++)
				sData << datac[i];
			
			delete[] datac;
		}
		else {
			sqlite3_finalize(dbStatement);
			sqlite3_close(dbConnection);
			return 2; //DatabaseHandler error: No object with id objId found
		}

		sqlite3_finalize(dbStatement);
		sqlite3_close(dbConnection);

		return 0;
	}
	
	int DatabaseHandler::initDatabase() {
		sqlCode = sqlite3_open(DB_NAME, &dbConnection);
		if(sqlCode != SQLITE_OK) { 
			sqlite3_close(dbConnection);
			return 1; //DatabaseHandler error: Error when opening database
		}
		
		const char* create1 = "CREATE TABLE objects (" \
			"id TEXT NOT NULL," \
			"className TEXT NOT NULL," \
			"data BLOB NOT NULL);";		// TODO: change to TEXT NOT NULL later
		sqlite3_prepare(dbConnection, create1, strlen(create1), &dbStatement, 0);
		sqlCode = sqlite3_step(dbStatement);
		if(sqlCode != SQLITE_DONE) {
			sqlite3_finalize(dbStatement);
			sqlite3_close(dbConnection);
			return 2; // DatabaseHandler error: Error creating table objects
		}
		
		sqlite3_finalize(dbStatement);
		sqlite3_close(dbConnection);
		
		return 0;
	}
	
	int DatabaseHandler::dropDatabase() {
		sqlCode = sqlite3_open(DB_NAME, &dbConnection);
		if(sqlCode != SQLITE_OK) {
			sqlite3_close(dbConnection);
			return 1; // DatabaseHandler error: Error when opening database
		}
		
		const char* drop = "DROP TABLE objects;";
		sqlite3_prepare(dbConnection, drop, strlen(drop), &dbStatement, 0);
		sqlCode = sqlite3_step(dbStatement);
		if(sqlCode != SQLITE_DONE) {
			sqlite3_finalize(dbStatement);
			sqlite3_close(dbConnection);
			return 2; // DatabaseHandler error: Error dropping table objects
		}
		
		sqlite3_finalize(dbStatement);
		sqlite3_close(dbConnection);
		
		return 0;
	}
}