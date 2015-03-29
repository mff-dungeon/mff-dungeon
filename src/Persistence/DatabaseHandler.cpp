#include "DatabaseHandler.hpp"
#include "Relation.hpp"
#include <unistd.h>
#include <cstring>
#include <string>
#include <cstdlib>

namespace Dungeon {

	DatabaseHandler& DatabaseHandler::getInstance() {	
		// get Singleton Instance
		static DatabaseHandler instance;
		return instance;
	}

	bool DatabaseHandler::openConnection() {
		sqlCode = sqlite3_open(Config::DbName().c_str(), &dbConnection);
		if (sqlCode != SQLITE_OK) {
			sqlite3_close(dbConnection);
			LOGS(Error) << "Couldn't open connection, SQLite error code " << sqlCode << "." << LOGF;
			return false;
		}
		LOGS(Debug) << "Database connection opened." << LOGF;
		return true;
	}

	DatabaseHandler::DatabaseHandler() {
		openConnection();
	}

	bool DatabaseHandler::closeConnection() {
		sqlCode = sqlite3_close(dbConnection);
		LOGS(Debug) << "Database connection closed." << LOGF;
		return true;
	}

	DatabaseHandler::~DatabaseHandler() {
		closeConnection();
	}

	void DatabaseHandler::finalize() {
		sqlite3_finalize(dbStatement);
	}

	// Saves object from stringstream into DB, returns 0 if successful

	int DatabaseHandler::saveObject(objId oid, string cName, string cData) {

		// Count, to find out if we should delete or update
		const char *countstate = "SELECT COUNT(*) FROM objects WHERE id = ?;";
		const char *coid = oid.c_str();
		sqlite3_prepare_v2(dbConnection, countstate, (int) strlen(countstate), &dbStatement, 0);
		sqlite3_bind_text(dbStatement, 1, coid, strlen(coid), 0);
		sqlCode = sqlite3_step(dbStatement);
		int count = 0;
		if (sqlCode == SQLITE_ROW) {
			count = sqlite3_column_int(dbStatement, 0);
			finalize();
		} else {
			finalize();
			LOGS(Error) << "Couldn't count table rows, SQLite error code " << sqlCode << "." << LOGF;
			return E_COUNT_ERROR;
		}
		if (count == 0) {
			const char *statement = "INSERT INTO objects (id, className, data) VALUES (?, ?, ?);";
			sqlite3_prepare_v2(dbConnection, statement, (int) strlen(statement), &dbStatement, 0);
		} else {
			const char *statement = "UPDATE objects SET id = ?, className = ?, data = ? WHERE id = ?;";
			sqlite3_prepare_v2(dbConnection, statement, (int) strlen(statement), &dbStatement, 0);
		}
		sqlite3_bind_text(dbStatement, 1, oid.c_str(), strlen(oid.c_str()), 0);
		sqlite3_bind_text(dbStatement, 2, cName.c_str(), strlen(cName.c_str()), 0);
		sqlite3_bind_blob(dbStatement, 3, cData.c_str(), cData.size(), 0);
		if (count != 0) sqlite3_bind_text(dbStatement, 4, oid.c_str(), strlen(oid.c_str()), 0);

		sqlCode = sqlite3_step(dbStatement);
		if (sqlCode != SQLITE_DONE) {
			finalize();
			LOGS(Error) << "Update query failed while saving an object, SQLite error code " << sqlCode << "." << LOGF;
			return E_UPDATE_ERROR;
		}
		finalize();
		LOGS(Debug) << "Updated object with id " << oid << "." << LOGF;
		return E_OK;
	}

	// Loading object into a stringstream, returns 0 if loaded successfuly

	int DatabaseHandler::loadObject(objId oid, string& cName, stringstream& sData) {
		const char *state = "SELECT * FROM objects WHERE id = ?;";
		const char *coid = oid.c_str();
		sqlite3_prepare_v2(dbConnection, state, (int) strlen(state), &dbStatement, 0);
		sqlite3_bind_text(dbStatement, 1, coid, strlen(coid), 0);
		sqlCode = sqlite3_step(dbStatement);
		if (sqlCode == SQLITE_ROW) {
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
			for (int i = 0; i < size; i++)
				sData << datac[i];

			delete[] datac;
		} else {
			finalize();
			LOGS(Debug) << "No object with id " << oid << " found." << LOGF;
			return E_NO_ROW; //DatabaseHandler error: No object with id objId found
		}

		finalize();
		LOGS(Debug) << "Found object with id " << oid << "." << LOGF;
		return E_OK;
	}

	int DatabaseHandler::getObjectList(vector<objId>& list) {
		// Loading object into a stringstream, returns 0 if loaded successfuly
		const char *state = "SELECT id FROM objects;";
		sqlite3_prepare_v2(dbConnection, state, (int) strlen(state), &dbStatement, 0);
		sqlCode = sqlite3_step(dbStatement);
		while (sqlCode == SQLITE_ROW) {
			list.push_back(objId((char *) sqlite3_column_text(dbStatement, 0)));
			sqlCode = sqlite3_step(dbStatement);
		}

		finalize();
		LOGS(Debug) << "Queried an object list." << LOGF;
		return E_OK;
	}

	int DatabaseHandler::deleteObject(objId oid) {
		const char* cquery = "DELETE FROM objects WHERE id = ?;";

		sqlite3_prepare_v2(dbConnection, cquery, strlen(cquery), &dbStatement, 0);
		sqlite3_bind_text(dbStatement, 1, oid.c_str(), strlen(oid.c_str()), 0);
		sqlCode = sqlite3_step(dbStatement);

		finalize();
		if (sqlCode != SQLITE_DONE) {
			LOGS(Error) << "Deleting object with id " << oid << " failed, SQLite error code " << sqlCode << "." << LOGF;
			return E_DELETE_ERROR;
		}
		LOGS(Debug) << "Deleted object with id " << oid << "." << LOGF;
		return E_OK;
	}

	int DatabaseHandler::getRelations(vector<Relation*>& result, Relation* rel) {
		const string qtmp = rel->getSelectQuery();
		// None parameters were handed, that is weird...
		if (qtmp == "") {
			sqlite3_close(dbConnection);
			LOGS(Warning) << "Attempted to execute an invalid getRelations query." << LOGF;
			return E_INVALID_QUERY;
		}

		// Do the real query and fill in the vector
		const char* cquery = qtmp.c_str();
		sqlite3_prepare(dbConnection, cquery, strlen(cquery), &dbStatement, 0);
		sqlCode = sqlite3_step(dbStatement);
		while (sqlCode == SQLITE_ROW) {
			Relation* r = new Relation(objId((char *) sqlite3_column_text(dbStatement, 0)),
					objId((char *) sqlite3_column_text(dbStatement, 1)), // FIXME: Leaks!!!
					string((char *) sqlite3_column_text(dbStatement, 2))); // FIXME: Leaks!!! leaves unallocated memory somehow
			result.push_back(r);
			sqlCode = sqlite3_step(dbStatement);
		}
		finalize();
		LOGS(Debug) << "Relations query executed." << LOGF;
		return E_OK;
	}

	int DatabaseHandler::addRelation(Relation* rel) {
		const char *statement = "INSERT INTO relations (mid, sid, relation) VALUES (?, ?, ?)";
		sqlite3_prepare_v2(dbConnection, statement, (int) strlen(statement), &dbStatement, 0);

		sqlite3_bind_text(dbStatement, 1, rel->mid.c_str(), strlen(rel->mid.c_str()), 0);
		sqlite3_bind_text(dbStatement, 2, rel->sid.c_str(), strlen(rel->sid.c_str()), 0);
		sqlite3_bind_text(dbStatement, 3, rel->relation.c_str(), strlen(rel->relation.c_str()), 0);

		sqlCode = sqlite3_step(dbStatement);
		if (sqlCode != SQLITE_DONE) {
			finalize();
			LOGS(Error) << "Relation inserting failed, SQLite error code " << sqlCode << "." << LOGF;
			return E_UPDATE_ERROR;
		}
		finalize();
		LOGS(Debug) << "Relation " << rel->relation << " (" << rel->mid << " -> " << rel->sid << ") successfully added." << LOGF;
		return E_OK;
	}

	int DatabaseHandler::deleteRelation(Relation* rel) {
		const string qtmp = rel->getDeleteQuery();
		// None parameters were handed, that is weird...
		if (qtmp == "") {
			sqlite3_close(dbConnection);
			return E_INVALID_QUERY;
		}

		const char* cquery = qtmp.c_str();

		sqlite3_prepare(dbConnection, cquery, strlen(cquery), &dbStatement, 0);
		sqlCode = sqlite3_step(dbStatement);

		finalize();
		if (sqlCode != SQLITE_DONE) {
			LOGS(Error) << "Relation removing failed, SQLite error code " << sqlCode << "." << LOGF;
			return E_DELETE_ERROR;
		}
		LOGS(Debug) << "Relations were successfully removed." << LOGF;
		return E_OK;
	}

	int DatabaseHandler::hasRelation(Relation* rel, bool& found) {
		const string qtmp = rel->getSelectQuery();
		if (qtmp == "") {
			sqlite3_close(dbConnection);
			LOGS(Warning) << "Attempted to execute an invalid hasRelation query." << LOGF;
			return E_INVALID_QUERY;
		}

		const char* cquery = qtmp.c_str();
		sqlite3_prepare(dbConnection, cquery, strlen(cquery), &dbStatement, 0);
		sqlCode = sqlite3_step(dbStatement);

		if (sqlCode == SQLITE_ROW) {
			found = true;
		} else {
			found = false;
		}

		finalize();
		LOGS(Debug) << "Executed a hasRelation query." << LOGF;
		return E_OK;
	}

	int DatabaseHandler::checkDatabase() {
		const char* check = "SELECT COUNT(*) FROM sqlite_master WHERE type = ?;";
		sqlite3_prepare_v2(dbConnection, check, strlen(check), &dbStatement, 0);
		sqlite3_bind_text(dbStatement, 1, "table", strlen("table"), 0);
		int sqlCode = sqlite3_step(dbStatement);

		if (sqlCode == SQLITE_ROW) {
			int count = sqlite3_column_int(dbStatement, 0);
			finalize();
			LOGS(Verbose) << "Executed count query, found " << count << " database tables." << LOGF;
			if (count == TABLE_COUNT) {
				return E_OK;
			} else {
				return E_TABLE_COUNT_ERROR;
			}
		} else {
			finalize();
			LOGS(Error) << "Couldn't count table rows, SQLite error code " << sqlCode << "." << LOGF;
			return E_COUNT_ERROR;
		}
	}

	int DatabaseHandler::initDatabase() {
		const char* create1 = "CREATE TABLE objects (" \
			"id TEXT NOT NULL," \
			"className TEXT NOT NULL," \
			"data BLOB NOT NULL);";
		const char* create2 = "CREATE TABLE relations (" \
			"mid TEXT NOT NULL," \
			"sid TEXT NOT NULL," \
			"relation TEXT NOT NULL);";
		// Parent id, type, Slave id, type, type of relation

		// Create objects table
		sqlite3_prepare(dbConnection, create1, strlen(create1), &dbStatement, 0);
		sqlCode = sqlite3_step(dbStatement);
		if (sqlCode != SQLITE_DONE) {
			finalize();
			LOGS(Error) << "Couldn't create database table, SQLite error code " << sqlCode << "." << LOGF;
			return E_TABLE_CREATE_ERROR;
		}
		sqlite3_finalize(dbStatement);

		// Create relations table
		sqlite3_prepare(dbConnection, create2, strlen(create2), &dbStatement, 0);
		sqlCode = sqlite3_step(dbStatement);
		if (sqlCode != SQLITE_DONE) {
			finalize();
			LOGS(Error) << "Couldn't create database table, SQLite error code " << sqlCode << "." << LOGF;
			return E_TABLE_CREATE_ERROR;
		}

		sqlite3_close(dbConnection);
		LOG << "Database tables created." << LOGF;
		return E_OK;
	}

	int DatabaseHandler::dropDatabase() {
		if (!openConnection()) return E_CONNECTION_ERROR;
		const char* drop1 = "DROP TABLE IF EXISTS objects;";
		const char* drop2 = "DROP TABLE IF EXISTS relations;";
		sqlite3_prepare(dbConnection, drop1, strlen(drop1), &dbStatement, 0);
		sqlCode = sqlite3_step(dbStatement);
		if (sqlCode != SQLITE_DONE) {
			finalize();
			LOGS(Error) << "Couldn't drop database table, SQLite error code " << sqlCode << "." << LOGF;
			return E_TABLE_DROP_ERROR;
		}
		sqlite3_finalize(dbStatement);
		sqlite3_prepare(dbConnection, drop2, strlen(drop2), &dbStatement, 0);
		sqlCode = sqlite3_step(dbStatement);
		if (sqlCode != SQLITE_DONE) {
			finalize();
			LOGS(Error) << "Couldn't drop database table, SQLite error code " << sqlCode << "." << LOGF;
			return E_TABLE_DROP_ERROR;
		}

		finalize();
		LOG << "Database tables dropped." << LOGF;
		return E_OK;
	}

	/**
	 * TODO implement these.
	 * I think that connection must be held open in between.
	 * FIXME! Really? But it's working now, eh?
	 */
	int DatabaseHandler::beginTransaction() {
		if (transactionDepth++ == 0) {
			sqlite3_exec(dbConnection, "BEGIN TRANSACTION;", NULL, NULL, NULL);
		}
		return E_OK;
	}

	int DatabaseHandler::endTransaction() {
		if (--transactionDepth == 0) {
			sqlite3_exec(dbConnection, "COMMIT;", NULL, NULL, NULL);
		}
		return E_OK;
	}

	int DatabaseHandler::rollback() {
		if (--transactionDepth == 0) {
			sqlite3_exec(dbConnection, "ROLLBACK;", NULL, NULL, NULL);
		}
		return E_OK;
	}
}
