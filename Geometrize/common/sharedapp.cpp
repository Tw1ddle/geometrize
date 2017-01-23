#include "common/sharedapp.h"

#include "image/imagelookup.h"
#include "job/joblookup.h"
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
    SharedAppImpl() : m_recentFiles{RecentItems::RECENTLY_OPENED_ITEMS_SETTINGS_GROUP, RecentItems::MAX_RECENTLY_OPENED_ITEMS_COUNT}
    {
    }

    SharedAppImpl& operator=(const SharedAppImpl&) = delete;
    SharedAppImpl(const SharedAppImpl&) = delete;
    ~SharedAppImpl() = default;

    RecentItems& getRecentFiles()
    {
        return m_recentFiles;
    }

    job::JobLookup& getJobLookup()
    {
        return m_jobLookup;
    }

    image::ImageLookup& getImageLookup()
    {
        return m_imageLookup;
    }

private:
    RecentItems m_recentFiles;
    job::JobLookup m_jobLookup;
    image::ImageLookup m_imageLookup;
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

job::JobLookup& SharedApp::getJobLookup()
{
    return d->getJobLookup();
}

image::ImageLookup& SharedApp::getImageLookup()
{
    return d->getImageLookup();
}

}

}

}
