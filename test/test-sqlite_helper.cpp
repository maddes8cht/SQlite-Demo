#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include "../src/sqlite-helper.h"

TEST_CASE("SqliteHelper - Open Database", "[SqliteHelper]") {
    const char* testDbName = "test_db.sqlite";
    try {
        SqliteHelper sqliteHelper(testDbName);
        // If no exception is thrown, the test case is considered successful
    } catch (const SQLiteException& ex) {
        FAIL("Exception caught: " << ex.what());
    }
}

TEST_CASE("SqliteHelper - Execute SQL Statement", "[SqliteHelper]") {
    const char* testDbName = "test_db.sqlite";
    SqliteHelper sqliteHelper(testDbName);

    const char* createTableSQL = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, name TEXT);";
    REQUIRE_NOTHROW(sqliteHelper.executeSqlStatement(createTableSQL));
}

TEST_CASE("SqliteHelper - Execute SELECT Statement", "[SqliteHelper]") {
    const char* testDbName = "test_db.sqlite";
    SqliteHelper sqliteHelper(testDbName);

    const char* insertDataSQL = "INSERT INTO test_table (name) VALUES ('John'), ('Alice'), ('Bob');";
    sqliteHelper.executeSqlStatement(insertDataSQL);

    const char* selectDataSQL = "SELECT * FROM test_table;";
    auto result = sqliteHelper.executeSelectStatement(selectDataSQL);

    REQUIRE(result.size() == 3);
    REQUIRE(result[0].size() == 2);
}
