#pragma once
#include "sqlite3.h"

class Database {
private:
    sqlite3* db;
    char* zErrMsg;
    int rc;

public:

    int OpenDatabaseConnection() {
        rc = sqlite3_open("test.db", &db);

        if (rc != SQLITE_OK) { // any number other than 0 is SQLITE_OK
            //fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            return(1);
        } else {
            //fprintf(stderr, "Opened database successfully\n");
        }
        return rc;
    }

    int CloseDatabaseConnection() {
        int result = sqlite3_close(db);
        return result;
    }


};

