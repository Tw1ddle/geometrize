#include "common/sharedapp.h"

#include "preferences/globalpreferences.h"
#include "recents/recentitems.h"

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
    SharedAppImpl() :
        m_recentFiles{RecentItems::RECENTLY_OPENED_ITEMS_SETTINGS_GROUP, RecentItems::MAX_RECENTLY_OPENED_ITEMS_COUNT},
        m_globalPreferences{preferences::getGlobalPreferencesConfigPath()}
    {
    }

    SharedAppImpl& operator=(const SharedAppImpl&) = delete;
    SharedAppImpl(const SharedAppImpl&) = delete;
    ~SharedAppImpl() = default;

    RecentItems& getRecentFiles()
    {
        return m_recentFiles;
    }

    preferences::GlobalPreferences& getGlobalPreferences()
    {
        return m_globalPreferences;
    }

private:
    RecentItems m_recentFiles;
    preferences::GlobalPreferences m_globalPreferences;
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

preferences::GlobalPreferences& SharedApp::getGlobalPreferences()
{
    return d->getGlobalPreferences();
}

}

}

}
