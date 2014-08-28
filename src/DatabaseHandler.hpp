#include <string>
#include <sstream>
#include <sqlite3.h>
#include "common.hpp"
#include "Relation.hpp"

#ifndef DATABASEHANDLER_HPP
#define	DATABASEHANDLER_HPP

using namespace std;

namespace Dungeon {
	
        /*
         * This class is supposed to handle all the database connection.
         * It is implemented as a singleton class.
         * WARNING: The class is implemented considering only single thread access.
         */
	class DatabaseHandler {
	private:
		sqlite3* dbConnection;
		sqlite3_stmt* dbStatement;
		int sqlCode;
		
		/**
		 * Establishes a database connection
         * @return true if connection was established, false otherwise
         */
		bool openConnection();
		
		/**
		 * Finalizes currently opened dbStatement and closes the connection
         */
		void finalizeAndClose();
		
		DatabaseHandler() { }		// Private constructor
		DatabaseHandler(const DatabaseHandler&);
		DatabaseHandler& operator=(const DatabaseHandler&);
		~DatabaseHandler() {};
	public:
		/**
		 * Returns the current opened instance of DatabaseHandler, opens a new 
		 * one if there is none
         * @return current instance of DatabaseHandler
         */
		static DatabaseHandler& getInstance();
		
		static const int E_OK = 0; // Query successfull
		static const int E_CONNECTION_ERROR = 1; // Unable to connect to database
		static const int E_NO_ROW = 3; // No row was found
		static const int E_UPDATE_ERROR = 5; // Unable to do an update query
		static const int E_DELETE_ERROR = 6; // Unable to do a delete query
		static const int E_TABLE_CREATE_ERROR = 10; // Unable to create a table
		static const int E_TABLE_DROP_ERROR = 11; // Unable to delete a table
		static const int E_TABLE_COUNT_ERROR = 12; // Not found expected number of tables
		static const int E_COUNT_ERROR = 15; // Unable to count rows
		static const int E_INVALID_QUERY = 20; // Invalid DB query
		
		static const int TABLE_COUNT = 2; // The number of expected tables
		
		/**
		 * Saves an object into database. Performs an insert query if the object
		 * is not there yet, update query otherwise
         * @param oid objectId
         * @param cName class name of the object
         * @param data object data saved in a string
         * @return DatabaseHandler state value
         */
		int saveObject(objId oid, string cName, string data);
		
		/**
		 * Loads an object from the database. 
         * @param oid objectId of the desired object
         * @param cName className of the desired object
         * @param sData stringstream which will be used to store the object
         * @return DatabaseHandler state value
         */
		int loadObject(objId oid, string& cName, stringstream& sData);
		
		/**
		 * Deletes an object from the database
         * @param oid objectId of the deleted object
         * @return DatabaseHandler state value
         */
		int deleteObject(objId oid);
		
		/**
		 * Checks whether the database has correct table count. 
		 * If not, something may be wrong / database is not set -> signal to 
		 * init db
         * @return DatabaseHandler state value
         */
		int checkDatabase();
		
		/**
		 * Initializes the database by creating the table structure
         * @return DatabaseHandler state value
         */
		int initDatabase();
		
		/**
		 * Fully drops the database
         * @return DatabaseHandler state value
         */
		int dropDatabase();

		/**
		 * Asks the database for the list of objects using 
		 * supplied Relation object for the query
         * @param result vector of matching relations
         * @param rel Relation required
         * @return DatabaseHandler state value
         */
		int getRelations(vector<Relation*>& result, Relation* rel);
		
		/**
		 * Adds a new relation to the database
         * @param rel Relation to be added
         * @return DatabaseHandler state value
         */
		int addRelation(Relation* rel);
		
		/**
		 * Removes relation from the database matching the supplied values
         * @param rel Relation to be removed
         * @return DatabaseHandler state value
         */
		int deleteRelation(Relation* rel);
	};
}
#endif

