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
 * @brief debugBreak Forces the debugger to break/halt the application when this is called.
 */
void debugBreak();

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
 * @brief getFilesWithExtension Gets the absolute file paths to the files with the given extension in the given directory.
 * @param dirPath The directory to search.
 * @param extension The file extension to match.
 * @return The file paths to the files encountered, empty vector if there are no matching files. The order of the file paths is implementation-dependent.
 */
std::vector<std::string> getFilesWithExtension(const std::string& dirPath, const std::string& extension);

/**
 * @brief getFirstFileWithExtension Gets the absolute file path to the first file with the given extension in the given directory.
 * @param dirPath The directory to search.
 * @param extension The file extension to match.
 * @return The file path to the first matching file encountered, empty string if there are no matching files. The 'first' file is implementation-dependent.
 */
std::string getFirstFileWithExtension(const std::string& dirPath, const std::string& extension);

/**
 * @brief getFirstFileWithExtensions Gets the absolute file path to the first file with one of the given extensions in the given directory.
 * @param dirPath The directory to search.
 * @param extensions The file extension to match.
 * @return The file path to the first matching file encountered, empty string if there are no matching files. The 'first' file is implementation-dependent.
 */
std::string getFirstFileWithExtensions(const std::string& dirPath, const std::vector<std::string>& extensions);

/**
 * @brief getScriptsForPath Gets the absolute file paths to the script files (*.chai) for the given directory, using recursive search.
 * @param dirPath The directory to search.
 * @return The absolute file paths to the script files for the given directory.
 */
std::vector<std::string> getScriptsForPath(const std::string& path);

/**
 * @brief getTemplateFoldersForPath Gets the absolute directory paths to the folders containing script files (*.chai) for the given directory, using recursive search.
 * @param dirPath The directory to search.
 * @return The absolute directory paths to the script files for the given directory.
 */
std::vector<std::string> getTemplateFoldersForPath(const std::string& dirPath);

/**
 * @brief getDirectoryForFilePath Gets the directory path from the given local file path.
 * @param filePath The filepath to chop.
 * @return The directory path i.e. the file path with the file name and any trailing slashes removed.
 */
std::string getDirectoryForFilePath(const std::string& filePath);

/**
 * @brief openDirectoryInDefaultExplorer Opens the given directory in the default file explorer.
 * @param dirPath The directory to open.
 * @return True if the directory exists and should open, else false.
 */
bool openDirectoryInDefaultExplorer(const std::string& dirPath);

}

}
