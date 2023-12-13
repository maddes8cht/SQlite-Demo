/**
 * @file sqlite-demo.cpp
 * @brief Demonstrates the usage of the SqliteHelper class to interact with an SQLite database.
 * @author Mathias Bachmann
 * @version 0.1
 */

#include "sqlite-helper.h"

/**
 * @brief Main function demonstrating SQLite database interactions.
 * @return 0 on success, 1 on SQLite exception.
 */

int main() {
    try {
        SqliteHelper sqliteHelper("example.db");

        // Create Table SQL Query
        const char *createTableSQL = "CREATE TABLE IF NOT EXISTS example_table (id INTEGER PRIMARY KEY, name TEXT, age INTEGER);";
        sqliteHelper.executeSqlStatement(createTableSQL);

        // Insert Data SQL Query
        const char *insertDataSQL = "INSERT INTO example_table (name, age) VALUES ('John', 25), ('Alice', 30), ('Bob', 22);";
        sqliteHelper.executeSqlStatement(insertDataSQL);

        SqliteResult result = sqliteHelper.executeSqlStatement("SELECT * FROM example_table;");
        printf("print result:\n");
        result.print(); // Print the result if it's a query
        printf("done\n");
        /*
                // Execute SELECT Query and Print Data
                const char *selectDataSQL = "SELECT * FROM example_table;";
                auto result = sqliteHelper.executeSelectStatement(selectDataSQL);
        */


    } catch (const SQLiteException &ex) {
        std::cerr << "SQLite Exception: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
