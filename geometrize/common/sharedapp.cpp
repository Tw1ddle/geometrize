#include "common/sharedapp.h"

#include "image/imagelookup.h"
#include "preferences/globalpreferences.h"
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

    image::ImageLookup& getImageLookup()
    {
        return m_imageLookup;
    }

    preferences::GlobalPreferences& getGlobalPreferences()
    {
        return m_globalPreferences;
    }

private:
    RecentItems m_recentFiles;
    image::ImageLookup m_imageLookup;
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

image::ImageLookup& SharedApp::getImageLookup()
{
    return d->getImageLookup();
}

preferences::GlobalPreferences& SharedApp::getGlobalPreferences()
{
    return d->getGlobalPreferences();
}

}

}

}
