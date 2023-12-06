#include "sqlite-helper.h"

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
