#include <iostream>
#include <sqlite3.h>

/**
 * @brief Main function to demonstrate SQLite functionality.
 *
 * This program demonstrates basic SQLite operations, including:
 * - Opening a database
 * - Creating a table
 * - Inserting data into the table
 * - Executing a SELECT query and printing the results
 *
 * @return 0 on successful execution, non-zero on error.
 */
int main() {
    // SQLite Database Connection
    sqlite3 *db;
    int rc = sqlite3_open("example.db", &db);

    if (rc != SQLITE_OK) {
        std::cerr << "Error opening the database: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    // Create Table SQL Query
    const char *createTableSQL = "CREATE TABLE IF NOT EXISTS example_table (id INTEGER PRIMARY KEY, name TEXT, age INTEGER);";

    // Create the table
    rc = sqlite3_exec(db, createTableSQL, 0, 0, 0);
    if (rc != SQLITE_OK) {
        std::cerr << "Error creating the table: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return rc;
    }

    // Insert Data SQL Query
    const char *insertDataSQL = "INSERT INTO example_table (name, age) VALUES ('John', 25), ('Alice', 30), ('Bob', 22);";

    // Insert data into the table
    rc = sqlite3_exec(db, insertDataSQL, 0, 0, 0);
    if (rc != SQLITE_OK) {
        std::cerr << "Error inserting data: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return rc;
    }

    // Execute SELECT Query and Print Data
    const char *selectDataSQL = "SELECT * FROM example_table;";

    sqlite3_exec(db, selectDataSQL, [](void *data, int argc, char **argv, char **colName) -> int {
        for (int i = 0; i < argc; i++) {
            std::cout << colName[i] << ": " << (argv[i] ? argv[i] : "NULL") << " | ";
        }
        std::cout << std::endl;
        return 0;
    }, 0, 0);

    // Close the database connection
    sqlite3_close(db);

    return 0;
}
