/**
 * @file sqlite-helper.h
 * @brief Declaration of the SqliteHelper class and related exceptions.
 *
 * Defines the SqliteHelper class for interacting with SQLite databases.
 * Defines SQLiteException class for handling SQLite-related errors.
 */

#ifndef SQLITE_HELPER_H
#define SQLITE_HELPER_H

#include <cstring>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sqlite3.h>
#include <stdexcept>
#include <vector>

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
     * @param[in] message The error message.
     */
    SQLiteException(const std::string &message) : std::runtime_error(message) {}
};

/**
 * @class SqliteHelper
 * @brief Provides functionality to interact with SQLite databases.
 *
 * The SqliteHelper class encapsulates common SQLite operations, such as opening a database,
 * executing SQL statements, and retrieving results from SELECT queries.
 */
class SqliteHelper {
  public:
    /**
     * @brief Constructor for SqliteHelper.
     * Destroys the SqliteHelper object and closes the SQLite database connection.
     * @param[in] dbName Name of the SQLite database file.
     */
    SqliteHelper(const char *dbName);

    /**
     * @brief Destructor for SqliteHelper.
     */
    ~SqliteHelper();

    /**
     * @brief Executes an SQL statement that does not return results (e.g., INSERT, UPDATE, DELETE).
     * @param[in] sqlStatement The SQL statement to be executed.
     * @throw SQLiteException if the execution of the statement fails.
     */
    void executeSqlStatement(const char *sqlStatement);

    /**
     * @brief Executes an SQL SELECT statement and returns the result as a vector of vectors of strings.
     * @param[in] selectStatement The SELECT SQL statement to be executed.
     * @return A vector of vectors of strings representing the result set.
     * @throw SQLiteException if the execution of the statement fails.
     */
    std::vector<std::vector<std::string>> executeSelectStatement(const char *selectStatement);

    /**
     * @brief Prints the result of an SQL SELECT statement with proper formatting.
     * @param[in] result The result set to be printed.
     * @param[in] columnWidths The width of each column for proper formatting.
     */
    void printSqlResult(const std::vector<std::vector<std::string>> &result, const std::vector<int> &columnWidths);

  private:
    sqlite3 *db; ///< SQLite database connection.

    /**
     * @brief Opens a connection to an SQLite database.
     * @param[in] dbName Name of the database.
     * @throw SQLiteException if opening the database fails.
     */
    void openDatabase(const char *dbName);
};

#endif // SQLITE_HELPER_H
