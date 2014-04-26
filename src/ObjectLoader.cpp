#include <sqlite3.h>
#include <string>
#include <iostream>

#include "ObjectLoader.hpp"
#include "Objects/Dummy.hpp"

namespace Dungeon {
	/*
	 * Just for now, will be probably changed soon
	 * DB_NAME is defined in common.hpp
	 * Testing table design: CREATE TABLE objects (oid TEXT NOT NULL, data TEXT NOT NULL);
	 */
	std::string ObjectLoader::loadFromDb(objId id) {

		int sqlCode;
		std::string result;
		
        sqlCode = sqlite3_open(DB_NAME, &dbConnection);
        if(sqlCode != SQLITE_OK) { sqlite3_close(dbConnection); } /* Fucking error happened */

        std::string stdstatement = "SELECT * FROM objects WHERE id = \"" + id + "\";";
        const char *statement = stdstatement.c_str();
		
        sqlite3_prepare(dbConnection, statement, 200, &dbStatement, NULL);

        sqlCode = sqlite3_step(dbStatement);
        if(sqlCode == SQLITE_ROW) {
            result = std::string((char *) sqlite3_column_text(dbStatement, 1));
        }
        else {
			/* No objects with this is were found */
			result = "";
        }
        sqlite3_finalize(dbStatement);
        sqlite3_close(dbConnection);
		return result;
	}
	
	IObject *ObjectLoader::loadObject(objId id) {
		std::string objValue = loadFromDb(id);
		return new DummyObject();
	}
	
	void ObjectLoader::saveObject(IObject *obj) {
	}
}