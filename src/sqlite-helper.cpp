/**
 * @file sqlite-helper.cpp
 * @brief Implementation of the SqliteHelper class.
 */

#include "sqlite-helper.h"

SqliteHelper::SqliteHelper(const char *dbName) : db(nullptr) {
    openDatabase(dbName);
}

SqliteHelper::~SqliteHelper() {
    if (db) {
        sqlite3_close(db);
    }
}

void SqliteHelper::openDatabase(const char *dbName) {
    int rc = sqlite3_open(dbName, &db);
    if (rc != SQLITE_OK) {
        throw SQLiteException("Error opening the database: " + std::string(sqlite3_errmsg(db)));
    }
}

void SqliteHelper::executeSqlStatement(const char *sqlStatement) {
    int rc = sqlite3_exec(db, sqlStatement, 0, 0, 0);
    if (rc != SQLITE_OK) {
        throw SQLiteException("Error executing SQL statement: " + std::string(sqlite3_errmsg(db)));
    }
}

std::vector<std::vector<std::string>> SqliteHelper::executeSelectStatement(const char *selectStatement) {
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, selectStatement, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        throw SQLiteException("Error preparing SELECT statement: " + std::string(sqlite3_errmsg(db)));
    }

    int columns = sqlite3_column_count(stmt);

    // Calculate maximum width of each column
    std::vector<int> columnWidths(columns, 0);

    // Iterate through the rows to find the maximum width of data in each column
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        for (int i = 0; i < columns; i++) {
            const char *columnValue = reinterpret_cast<const char *>(sqlite3_column_text(stmt, i));
            columnWidths[i] = std::max(columnWidths[i], static_cast<int>(columnValue ? strlen(columnValue) : 4)); // Minimum width is 4 for "NULL"
        }
    }

    // Reset the statement so that we can iterate through the rows again
    sqlite3_reset(stmt);

    // Create a local variable to store the results
    std::vector<std::vector<std::string>> result;

    // Print header with column names
    for (int i = 0; i < columns; i++) {
        const char *columnName = sqlite3_column_name(stmt, i);
        std::cout << std::setw(columnWidths[i]) << std::left << columnName << " | ";
    }
    std::cout << std::endl;

    // Print line of dashes
    for (int i = 0; i < std::accumulate(columnWidths.begin(), columnWidths.end(), 0) + 3 * (columns - 1); i++) {
        std::cout << "-";
    }
    std::cout << std::endl;

    // Print table
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        std::vector<std::string> row;

        for (int i = 0; i < columns; i++) {
            const char *columnValue = reinterpret_cast<const char *>(sqlite3_column_text(stmt, i));
            row.push_back(columnValue ? columnValue : "NULL");
            std::cout << std::setw(columnWidths[i]) << std::left << (columnValue ? columnValue : "NULL") << " | ";
        }
        std::cout << std::endl;

        result.push_back(row);
    }

    if (rc != SQLITE_DONE) {
        throw SQLiteException("Error executing SELECT statement: " + std::string(sqlite3_errmsg(db)));
    }

    sqlite3_finalize(stmt);

    return result;
}

void SqliteHelper::printSqlResult(const std::vector<std::vector<std::string>> &result, const std::vector<int> &columnWidths) {
    int columns = columnWidths.size();

    // Calculate the total width of the table
    int totalWidth = std::accumulate(columnWidths.begin(), columnWidths.end(), 0) + 3 * (columns - 1);

    // Print header with column names
    for (int i = 0; i < columns; i++) {
        std::cout << std::setw(columnWidths[i]) << std::left << result[0][i] << " | ";
    }
    std::cout << std::endl;

    // Print line of dashes
    for (int i = 0; i < totalWidth; i++) {
        std::cout << "-";
    }
    std::cout << std::endl;

    // Print table
    for (const auto &row : result) {
        for (int i = 0; i < columns; i++) {
            std::cout << std::setw(columnWidths[i]) << std::left << row[i] << " | ";
        }
        std::cout << std::endl;
    }
}
