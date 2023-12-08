#include "sqlite-helper.h"

void openDatabase(sqlite3 *&db, const char *dbName) {
    int rc = sqlite3_open(dbName, &db);
    if (rc != SQLITE_OK) {
        throw SQLiteException("Error opening the database: " + std::string(sqlite3_errmsg(db)));
    }
}

void executeSqlStatement(sqlite3 *db, const char *sqlStatement) {
    int rc = sqlite3_exec(db, sqlStatement, 0, 0, 0);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        throw SQLiteException("Error executing SQL statement: " + std::string(sqlite3_errmsg(db)));
    }
}

void executeAndPrintSelectStatement(sqlite3 *db, const char *selectStatement) {
    int rc = sqlite3_exec(db, selectStatement, [](void *data, int argc, char **argv, char **colName) -> int {
        for (int i = 0; i < argc; i++) {
            std::cout << colName[i] << ": " << (argv[i] ? argv[i] : "NULL") << " | ";
        }
        std::cout << std::endl;
        return 0;
    }, 0, 0);

    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        throw SQLiteException("Error executing SELECT statement: " + std::string(sqlite3_errmsg(db)));
    }
}
