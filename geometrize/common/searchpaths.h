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
 * @brief getScriptSearchPaths Gets the directory paths that the program will search for scripts.
 * These may include resources (":/resourcedir/"), relative ("../../templates"), or absolute directory paths ("C:/Users/admin/Desktop").
 * @return The vector of script search paths.
 */
std::vector<std::string> getScriptSearchPaths();

/**
 * @brief getTemplateSearchpaths Gets the directory paths that the program will search for project file templates.
 * These may include resources (":/resourcedir/"), relative ("../../templates"), or absolute directory paths ("C:/Users/admin/Desktop").
 * @return The vector of template search paths.
 */
std::vector<std::string> getTemplateSearchPaths();

/**
 * @brief getTaskSettingsFilename Gets the default name of a task settings file.
 * @return The default filename of a task settings file, including the file extension.
 */
std::string getTaskSettingsFilename();

}

}
