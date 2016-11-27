#include "versioninfo.h"

#include <QSysInfo>
#include <QtGlobal>

namespace geometrize
{

namespace version
{

QString getApplicationVersionString()
{
    return "1.0.0";
}

QString getRuntimeOperatingSystemVersionName()
{
#ifdef Q_OS_WIN
    switch(QSysInfo::windowsVersion()) {
    case QSysInfo::WV_2000: return "2000";
    case QSysInfo::WV_Me: return "ME";
    case QSysInfo::WV_XP: return "XP";
    case QSysInfo::WV_VISTA: return "Vista";
    case QSysInfo::WV_WINDOWS7: return "7";
    case QSysInfo::WV_WINDOWS8: return "8";
    case QSysInfo::WV_WINDOWS8_1: return "8.1";
    case QSysInfo::WV_WINDOWS10: return "10";
    default: return "Unknown";
    }
#elif Q_OS_OSX
    switch(QSysInfo::macVersion()) {
    case QSysInfo::MV_CHEETAH: return "Cheetah";
    case QSysInfo::MV_PUMA: return "Puma";
    case QSysInfo::MV_JAGUAR: return "Jaguar";
    case QSysInfo::MV_PANTHER: return "Panther";
    case QSysInfo::MV_TIGER: return "Tiger";
    case QSysInfo::MV_LEOPARD: return "Leopard";
    case QSysInfo::MV_SNOWLEOPARD: return "Snow Leopard";
    case QSysInfo::MV_LION: return "Lion";
    case QSysInfo::MV_MOUNTAINLION: return "Mountain Lion";
    case QSysInfo::MV_MAVERICKS: return "Mavericks";
    case QSysInfo::MV_YOSEMITE: return "Yosemite";
    case QSysInfo::MV_ELCAPITAN: return "El Capitan";
    default: return "Unknown";
    }
#elif Q_OS_LINUX
    return "Unknown";
#else
    static_assert(false, "Unrecognized OS, implement version name code for this target");
#endif
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
