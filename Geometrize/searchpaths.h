#pragma once

#include <string>
#include <vector>

namespace geometrize
{

namespace searchpaths
{

/**
 * @brief getApplicationDirectoryPath Gets the directory that contains the application executable.
 * @return The directory path that contains the application executable.
 */
std::string getApplicationDirectoryPath();

/**
 * @brief getScriptSearchPaths Gets the directory paths (relative to the application executable directory) that the program will search for scripts.
 * @return The vector of script search paths.
 */
std::vector<std::string> getScriptSearchPaths();

/**
 * @brief getTemplateSearchpaths Gets the directory paths (relative to the application executable directory) that the program will search for project file templates.
 * @return The vector of template search paths.
 */
std::vector<std::string> getTemplateSearchPaths();

}

}
