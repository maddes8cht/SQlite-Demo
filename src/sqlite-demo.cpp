/**
 * @file
 * @brief Example program demonstrating SQLite functionality.
 *
 * This program illustrates basic SQLite operations, including:
 * - Opening a database
 * - Creating a table
 * - Inserting data into the table
 * - Executing a SELECT query and printing the results
 *
 * @author Mathias Bachmann
 * @date 2023/12/05
 */

#include <iostream>
#include <stdexcept>
#include <sqlite3.h>

/**
 * @brief Custom exception class for SQLite-related errors.
 *
 * This exception class is derived from std::runtime_error and is used to handle errors
 * that occur during SQLite operations.
 */
class SQLiteException : public std::runtime_error {
public:
    /**
     * @brief Constructs an SQLiteException with a given error message.
     *
     * @param[in] message The error message.
     */
    SQLiteException(const std::string &message) : std::runtime_error(message) {}
};

/**
 * @brief SQLite Database Connection
 *
 * This function opens a SQLite database connection.
 *
 * @param[out] db SQLite database connection
 * @param[in] dbName Name of the database
 * @throw SQLiteException on failure
 */
void openDatabase(sqlite3 *&db, const char *dbName) {
    int rc = sqlite3_open(dbName, &db);
    if (rc != SQLITE_OK) {
        throw SQLiteException("Error opening the database: " + std::string(sqlite3_errmsg(db)));
    }
}

/**
 * @brief Execute SQL Statement
 *
 * This function executes a given SQL statement.
 *
 * @param[in] db SQLite database connection
 * @param[in] sqlStatement SQL statement to be executed
 * @throw SQLiteException on failure
 */
void executeSqlStatement(sqlite3 *db, const char *sqlStatement) {
    int rc = sqlite3_exec(db, sqlStatement, 0, 0, 0);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        throw SQLiteException("Error executing SQL statement: " + std::string(sqlite3_errmsg(db)));
    }

    // No need to close the database connection here; it's already done in the functions
}

/**
 * @brief Execute SELECT Statement and Print Results
 *
 * This function executes a SELECT SQL statement and prints the results.
 *
 * @param[in] db SQLite database connection
 * @param[in] selectStatement SELECT SQL statement to be executed
 * @throw SQLiteException on failure
 */
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

    // No need to close the database connection here; it's already done in the functions
}

/**
 * @brief Main function for the SQLite demonstration program.
 *
 * This function demonstrates basic SQLite operations:
 * - Opening a database
 * - Creating a table
 * - Inserting data into the table
 * - Executing a SELECT query and printing the results
 *
 * @return 0 on successful execution, 1 on error
 */
int main() {
    sqlite3 *db;

    try {
        // Open the database
        openDatabase(db, "example.db");

        // Create Table SQL Query
        const char *createTableSQL = "CREATE TABLE IF NOT EXISTS example_table (id INTEGER PRIMARY KEY, name TEXT, age INTEGER);";
        executeSqlStatement(db, createTableSQL);

        // Insert Data SQL Query
        const char *insertDataSQL = "INSERT INTO example_table (name, age) VALUES ('John', 25), ('Alice', 30), ('Bob', 22);";
        executeSqlStatement(db, insertDataSQL);

        // Execute SELECT Query and Print Data
        const char *selectDataSQL = "SELECT * FROM example_table;";
        executeAndPrintSelectStatement(db, selectDataSQL);

        // Close the database
        sqlite3_close(db);


    } catch (const SQLiteException &ex) {
        std::cerr << "SQLite Exception: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
