#pragma once

#include <QString>

namespace geometrize
{

namespace version
{

/**
 * @brief getApplicationVersionString Gets the application version as a string in VERSION.MAJOR.MINOR format e.g. "1.0.0", "2.7.13".
 * @return The application version as a string.
 */
QString getApplicationVersionString();

/**
 * @brief getRuntimeMachineHostName Gets the system's runtime host name, if one is set.
 * @return The system's host name.
 */
QString getRuntimeMachineHostName();

/**
 * @brief getRuntimeCpuArchitectureName Gets the architecture of the CPU that the application is currently running on.
 * @return The architecture of the CPU the application is currently running on.
 */
QString getRuntimeCpuArchitectureName();

/**
 * @brief getRuntimeQtVersionName Gets the version of Qt that the application is currently running on.
 * @return  The version of Qt that the application is currently running on.
 */
QString getRuntimeQtVersionName();

/**
 * @brief getBuildDateTime Gets a string that describes the date and time the application was built.
 * @return The date and time the application was built.
 */
QString getBuildDateTime();

/**
 * @brief getBuildOperatingSystemName Gets the name of the OS the application is built to run on e.g. "Windows", or "Linux".
 * @return The name of the OS the application is built to run on.
 */
QString getBuildOperatingSystemName();

/**
 * @brief getBuildAbiName Gets the full architecture that the application was compiled for.
 * @return The full architecture that the application was compiled for.
 */
QString getBuildAbiName();

/**
 * @brief getBuildCpuArchitectureName Gets the architecture of the CPU the application was compiled for.
 * @return The architecture of the CPU the application was compiled for.
 */
QString getBuildCpuArchitectureName();

/**
 * @brief getBuildQtVersionName Gets the version of Qt the application was compiled against.
 * @return The version of Qt the application was compiled against.
 */
QString getBuildQtVersionName();

/**
 * @brief getBuildQtLicenseName Gets a string that describes the sort of license Qt is provided under.
 * @return The sort of license Qt has been provided under.
 */
QString getBuildQtLicenseName();

}

}
