#ifndef RECENTFILES_H
#define RECENTFILES_H

#include <QObject>

namespace geometrize
{

/**
 * @brief The RecentFiles class encapsulates a list of files that were recently interacted with.
 * The class keeps a record of loaded files stored in preferences.
 */
class RecentFiles
{
public:
    // TODO load recents from preferences
    RecentFiles() = default;
    RecentFiles& operator=(const RecentFiles&) = delete;
    RecentFiles(const RecentFiles&) = delete;
    ~RecentFiles() = default;

    void add(const QString& filePath);
    void remove(const QString& filePath);
    void clear();

private:

};

}

#endif // RECENTFILES_H
