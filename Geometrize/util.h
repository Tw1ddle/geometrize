#pragma once

#include <string>
#include <vector>

namespace geometrize
{

namespace util
{

/**
 * @brief printToConsole A convenience function for printing a string to the command-line console
 * @param str The string to print to the console.
 */
void printToConsole(const std::string& str);

/**
 * @brief directoryExists Checks if a directory exists, returns true if it does.
 * @param dirPath The directory path.
 * @return True if the directory exists, false if it does not.
 */
bool directoryExists(const std::string& dirPath);

/**
 * @brief directoryContainsFile Checks if the directory contains the given file, returns true if it does.
 * @return True if the directory contains the given file, false otherwise.
 */
bool directoryContainsFile(const std::string& dirPath, const std::string& fileName);

/**
 * @brief readFileAsString Reads a file, returning a string containing the contents.
 * @param filePath The path to the file.
 * @return The contents of the file, as a string.
 */
std::string readFileAsString(const std::string& filePath);

/**
 * @brief getFilePathsForDirectory Gets the absolute file paths to the files in the given directory.
 * @param dirPath The directory to search.
 * @return A vector of files.
 */
std::vector<std::string> getFilePathsForDirectory(const std::string& dirPath);

/**
 * @brief getFirstFileWithExtension Gets the absolute file path to the first file with the given extension in the given directory.
 * @param dirPath The directory to search.
 * @param extension The file extension to match.
 * @return The file path to the first file encountered, empty string if there are no matching files. The 'first' file is implementation-dependent.
 */
std::string getFirstFileWithExtension(const std::string& dirPath, const std::string& extension);

/**
 * @brief getScriptsForPath Gets the absolute file paths to the script files (*.chai) for the given directory, using recursive search.
 * @param dirPath The directory to search.
 * @return The absolute file paths to the script files for the given directory.
 */
std::vector<std::string> getScriptsForPath(const std::string& path);

}

}
