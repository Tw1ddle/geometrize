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
 * @brief getTaskSettingsFilename Gets the default name of a task settings file that is manually saved by the user, or a task settings file bundled with a project file template.
 * @return The default filename of a task settings file, including the file extension.
 */
std::string getDefaultTaskSettingsFilename();

/**
 * @brief getAutomaticTaskSettingsFilename Gets the name of a task settings file to be autosaved (e.g. when the task window is closed).
 * @param hashOfFirstTargetImage A string containing a hash of the first target image the task was set up to use.
 * NOTE - this hash changes based on the target image, so modifying the "resize image" settings in the application will cause this to change.
 * @return The name of the autosave task settings file, including the file extension
 */
std::string getAutosaveTaskSettingsFilename(const std::string& hashOfFirstTargetImage);

/**
 * @brief getAutosaveTaskSettingsSearchPaths Gets the directory paths that the program will search for autosaved task settings files.
 * The list of directory paths returned is sorted from high to low priority and it should never be empty.
 * @return The vector of autosave task settings search paths.
 */
std::vector<std::string> getAutosaveTaskSettingsSearchPaths();

}

}
