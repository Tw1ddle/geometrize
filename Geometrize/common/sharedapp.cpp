#include "common/sharedapp.h"

#include "recentitems.h"

namespace geometrize
{

namespace common
{

namespace app
{

/**
 * @brief The SharedAppImpl class contains the concrete implementation of the SharedApp class.
 */
class SharedApp::SharedAppImpl : public QObject
{
public:
    SharedAppImpl() : m_recentFiles{RecentItems::RECENTLY_OPENED_ITEMS_SETTINGS_GROUP}
    {
    }

    SharedAppImpl& operator=(const SharedAppImpl&) = delete;
    SharedAppImpl(const SharedAppImpl&) = delete;
    ~SharedAppImpl() = default;

    RecentItems& getRecentFiles()
    {
        return m_recentFiles;
    }

private:
    RecentItems m_recentFiles;
};

SharedApp::SharedApp() : d{std::make_unique<geometrize::common::app::SharedApp::SharedAppImpl>()} {}
SharedApp::~SharedApp() {}

SharedApp& SharedApp::get()
{
    static SharedApp _instance;
    return _instance;
}

RecentItems& SharedApp::getRecentFiles()
{
    return d->getRecentFiles();
}

}

}

}
