#ifndef SQLITE_HELPER_H
#define SQLITE_HELPER_H

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
void openDatabase(sqlite3 *&db, const char *dbName);

/**
 * @brief Execute SQL Statement
 *
 * This function executes a given SQL statement.
 *
 * @param[in] db SQLite database connection
 * @param[in] sqlStatement SQL statement to be executed
 * @throw SQLiteException on failure
 */
void executeSqlStatement(sqlite3 *db, const char *sqlStatement);

/**
 * @brief Execute SELECT Statement and Print Results
 *
 * This function executes a SELECT SQL statement and prints the results.
 *
 * @param[in] db SQLite database connection
 * @param[in] selectStatement SELECT SQL statement to be executed
 * @throw SQLiteException on failure
 */
void executeAndPrintSelectStatement(sqlite3 *db, const char *selectStatement);

#endif // SQLITE_HELPER_H
