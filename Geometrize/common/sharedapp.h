#pragma once

#include <memory>

#include <QObject>

namespace geometrize
{
class RecentItems;
}

namespace geometrize
{

namespace common
{

namespace app
{

/**
 * @brief The SharedApp class is a lazy singleton that contains common core functionality.
 */
class SharedApp : public QObject
{
    Q_OBJECT
public:
    static SharedApp& get();
    SharedApp& operator=(const SharedApp&) = delete;
    SharedApp(const SharedApp&) = delete;

    // Recent files
    RecentItems& getRecentFiles();

private:
    class SharedAppImpl;
    std::unique_ptr<SharedAppImpl> d;

    SharedApp();
    ~SharedApp();
};

}

}

}
