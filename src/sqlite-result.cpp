/**
 * @file sqlite-result.cpp
 * @brief Implementation of the SqliteResult class.
 */

#include "sqlite-result.h"

SqliteResult::SqliteResult(const std::vector<std::vector<std::string>>& result, const std::vector<std::string>& header)
    : result_(result), header_(header) {}

SqliteResult::~SqliteResult() {}

size_t SqliteResult::getRowCount() const {
    return result_.size();
}

size_t SqliteResult::getColumnCount() const {
    if (!result_.empty()) {
        return result_[0].size();
    }
    return 0;
}

void SqliteResult::print() const {
    size_t columns = getColumnCount();

    // Print header with column names
    for (size_t i = 0; i < columns; i++) {
        std::cout << std::setw(15) << std::left << header_[i] << " | "; // Adjust width as needed
    }
    std::cout << std::endl;

    // Print line of dashes
    for (size_t i = 0; i < 17 * columns; i++) { // Adjust width as needed
        std::cout << "-";
    }
    std::cout << std::endl;

    // Print table
    for (const auto& row : result_) {
        for (size_t i = 0; i < columns; i++) {
            std::cout << std::setw(15) << std::left << row[i] << " | "; // Adjust width as needed
        }
        std::cout << std::endl;
    }
}
