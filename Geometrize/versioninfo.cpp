#include "versioninfo.h"

#include <QSysInfo>

namespace geometrize
{

namespace version
{

QString getApplicationVersionString()
{
    return "1.0.0";
}

QString getRuntimeMachineHostName()
{
    return QSysInfo::machineHostName();
}

QString getRuntimeCpuArchitectureName()
{
    return QSysInfo::currentCpuArchitecture();
}

QString getRuntimeQtVersionName()
{
    return qVersion();
}

QString getBuildDateTime()
{
    return QString(__DATE__) + " " + QString(__TIME__);
}

QString getBuildOperatingSystemName()
{
#ifdef Q_OS_WIN
    return "Windows";
#elif Q_OS_OSX
    return "OSX";
#elif Q_OS_LINUX
    return "Linux";
#else
    static_assert(false, "Unrecognized OS, implement operating system name code for this target");
#endif
}

QString getBuildAbiName()
{
    return QSysInfo::buildAbi();
}

QString getBuildCpuArchitectureName()
{
    return QSysInfo::buildCpuArchitecture();
}

QString getBuildQtVersionName()
{
    return QT_VERSION_STR;
}

QString getBuildQtLicenseName()
{
    return QT_PRODUCT_LICENSE;
}

}

}
