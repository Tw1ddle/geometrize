#pragma once

#include <string>

class QString;

namespace geometrize
{

namespace util
{

/**
 * @brief directoryExists Checks if a directory exists, returns true if it does.
 * @param dirPath The directory path.
 * @return True if the directory exists, false if it does not.
 */
bool directoryExists(const QString& dirPath);

/**
 * @brief directoryContainsFile Checks if the directory contains the given file, returns true if it does.
 * @return True if the directory contains the given file, false otherwise.
 */
bool directoryContainsFile(const QString& dirPath, const QString& fileName);

/**
 * @brief readFileAsString Reads a file, returning a string containing the contents.
 * @param filePath The path to the file.
 * @return The contents of the file, as a string.
 */
std::string readFileAsString(const QString& filePath);

}

}
