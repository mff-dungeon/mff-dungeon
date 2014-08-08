#include <cstring>
#include <string>
#include <cstdlib>

#include "DatabaseHandler.hpp"
#include "common.hpp"
#include "Relation.hpp"

namespace Dungeon {

	// get Singleton Instance
	DatabaseHandler& DatabaseHandler::getInstance() {
		static DatabaseHandler instance;
		return instance;
	}
	
	bool DatabaseHandler::openConnection() {
		sqlCode = sqlite3_open(DB_NAME, &dbConnection);
		if(sqlCode != SQLITE_OK) { 
			sqlite3_close(dbConnection);
			return false;
		}
		return true;
	}
	
	void DatabaseHandler::finalizeAndClose() {
		sqlite3_finalize(dbStatement);
		sqlite3_close(dbConnection);
	}

	// Saves object from stringstream into DB, returns 0 if successful
	int DatabaseHandler::saveObject(objId oid, string cName, string cData) {
		if(!openConnection()) return E_CONNECTION_ERROR;

		// Count, to find out if we should delete or update
		string scount = "SELECT COUNT(*) FROM objects WHERE id = " + oid + ";";
		const char *countstate = scount.c_str();
		sqlite3_prepare(dbConnection, countstate, 200, &dbStatement, 0);
		sqlCode = sqlite3_step(dbStatement);
		int count = 0;
		if(sqlCode == SQLITE_ROW) {
			count = sqlite3_column_int(dbStatement, 0);
		}
		else {
			finalizeAndClose();
			return E_COUNT_ERROR;
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
			finalizeAndClose();
			return E_UPDATE_ERROR;
		}
		finalizeAndClose();

		return E_OK;
	}

	// Loading object into a stringstream, returns 0 if loaded successfuly
	int DatabaseHandler::loadObject(objId oid, string& cName, stringstream& sData) {
		if(!openConnection()) return E_CONNECTION_ERROR;

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
			finalizeAndClose();
			return E_NO_ROW; //DatabaseHandler error: No object with id objId found
		}

		finalizeAndClose();
		return E_OK;
	}
	
	int DatabaseHandler::deleteObject(objId oid) {
		if(!openConnection()) return E_CONNECTION_ERROR;
		const char* cquery = "DELETE FROM objects WHERE oid = ?;";

		sqlite3_prepare_v2(dbConnection, cquery, strlen(cquery), &dbStatement, 0);
		sqlite3_bind_text(dbStatement, 1, oid.c_str(), strlen(oid.c_str()), 0);
		sqlCode = sqlite3_step(dbStatement);
		
		finalizeAndClose();
		if(sqlCode != SQLITE_DONE)
			return E_DELETE_ERROR;
		return E_OK;
	}

	int DatabaseHandler::getRelations(vector<objId>& result, Relation* rel) {
		if(!openConnection()) return E_CONNECTION_ERROR;
		const string qtmp = rel->getSelectQuery();
		// None parameters were handed, that is weird...
		if(qtmp == "") {
			sqlite3_close(dbConnection);
			return E_INVALID_QUERY;
		}
	
		// Do the real query and fill in the vector
		const char* cquery = qtmp.c_str();
		sqlite3_prepare(dbConnection, cquery, strlen(cquery), &dbStatement, 0);
		sqlCode = sqlite3_step(dbStatement);
		while(sqlCode == SQLITE_ROW) {
			string oid = std::string((char *) sqlite3_column_text(dbStatement, 0));
			result.push_back(oid);
			sqlCode = sqlite3_step(dbStatement);
		}
		finalizeAndClose();
		return E_OK;
	}

	int DatabaseHandler::addRelation(Relation* rel) {
		if(!openConnection()) return E_CONNECTION_ERROR;
		const char *statement = "INSERT INTO relations (pid, sid, pclass, sclass, relation) VALUES (?, ?, ?, ?, ?)";
		sqlite3_prepare_v2(dbConnection, statement, (int)strlen(statement), &dbStatement, 0);

		sqlite3_bind_text(dbStatement, 1, rel->pid.c_str(), strlen(rel->pid.c_str()), 0);
		sqlite3_bind_text(dbStatement, 2, rel->sid.c_str(), strlen(rel->sid.c_str()), 0);
		sqlite3_bind_text(dbStatement, 3, rel->pclass.c_str(), strlen(rel->pclass.c_str()), 0);
		sqlite3_bind_text(dbStatement, 4, rel->sclass.c_str(), strlen(rel->sclass.c_str()), 0);
		sqlite3_bind_text(dbStatement, 5, rel->relation.c_str(), strlen(rel->relation.c_str()), 0);

		sqlCode = sqlite3_step(dbStatement);
		if(sqlCode != SQLITE_DONE) { 
			finalizeAndClose();
			return E_UPDATE_ERROR;
		}
		finalizeAndClose();
		return E_OK;
	}

	int DatabaseHandler::deleteRelation(Relation* rel) {
		if(!openConnection()) return E_CONNECTION_ERROR;
		const string qtmp = rel->getDeleteQuery();
		// None parameters were handed, that is weird...
		if(qtmp == "") {
			sqlite3_close(dbConnection);
			return E_INVALID_QUERY; 
		}

		const char* cquery = qtmp.c_str();

		sqlite3_prepare(dbConnection, cquery, strlen(cquery), &dbStatement, 0);
		sqlCode = sqlite3_step(dbStatement);
		
		finalizeAndClose();
		if(sqlCode != SQLITE_DONE)
			return E_DELETE_ERROR;
		return E_OK;
	}
	
	int DatabaseHandler::initDatabase() {
		if(!openConnection()) return E_CONNECTION_ERROR;
		const char* create1 = "CREATE TABLE objects (" \
			"id TEXT NOT NULL," \
			"className TEXT NOT NULL," \
			"data BLOB NOT NULL);";		// TODO: change to TEXT NOT NULL later
		const char* create2 = "CREATE TABLE relations (" \
			"pid TEXT NOT NULL," \
			"pclass TEXT NOT NULL," \
			"sid TEXT NOT NULL," \
			"sclass TEXT NOT NULL, " \
			"relation TEXT NOT NULL);";     
		// Parent id, type, Son id, type, type of relation
                
		// Create objects table
		sqlite3_prepare(dbConnection, create1, strlen(create1), &dbStatement, 0);
		sqlCode = sqlite3_step(dbStatement);
		if(sqlCode != SQLITE_DONE) {
			finalizeAndClose();
			return E_TABLE_CREATE_ERROR;
		}
		sqlite3_finalize(dbStatement);
                
		// Create relations table
		sqlite3_prepare(dbConnection, create2, strlen(create2), &dbStatement, 0);
		sqlCode = sqlite3_step(dbStatement);
		if(sqlCode != SQLITE_DONE) {
			finalizeAndClose();
			return E_TABLE_CREATE_ERROR;
		}
                
		sqlite3_close(dbConnection);
		return E_OK;
	}
	
	int DatabaseHandler::dropDatabase() {
		if(!openConnection()) return E_CONNECTION_ERROR;
		const char* drop = "DROP TABLE IF EXISTS objects; DROP TABLE IF EXISTS relations;";
		sqlite3_prepare(dbConnection, drop, strlen(drop), &dbStatement, 0);
		sqlCode = sqlite3_step(dbStatement);
		if(sqlCode != SQLITE_DONE) {
		finalizeAndClose();
			return E_TABLE_DROP_ERROR;
		}
		
		finalizeAndClose();
		return E_OK;
	}
}
