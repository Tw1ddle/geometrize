#include "versioninfo.h"

#include <QLibraryInfo>
#include <QSysInfo>
#include <QtGlobal>

namespace geometrize
{

namespace version
{

QString getApplicationVersionString()
{
    return "1.0.1";
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
#endif

#ifdef Q_OS_MAC
    return "OSX";
#endif

#ifdef Q_OS_LINUX
    return "Linux";
#endif

    return "Unknown";
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

}

}
