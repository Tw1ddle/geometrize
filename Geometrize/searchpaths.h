#pragma once

#include <QString>
#include <QStringList>

namespace geometrize
{

namespace searchpaths
{

/**
 * @brief getApplicationDirectoryPath Gets the directory that contains the application executable.
 * @return The directory path that contains the application executable.
 */
QString getApplicationDirectoryPath();

/**
 * @brief getScriptSearchPaths Gets the directory paths (relative to the application executable directory) that the program will search for scripts.
 * @return The list of script search paths.
 */
QStringList getScriptSearchPaths();

/**
 * @brief getTemplateSearchpaths Gets the directory paths (relative to the application executable directory) that the program will search for project file templates.
 * @return The list of template search paths.
 */
QStringList getTemplateSearchpaths();

}

}
