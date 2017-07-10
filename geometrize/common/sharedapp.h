#pragma once

#include <memory>

namespace geometrize
{
class RecentItems;
}

namespace geometrize
{

namespace job
{
class JobLookup;
}

namespace image
{
class ImageLookup;
}

namespace preferences
{
class GlobalPreferences;
}

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
class SharedApp
{
public:
    static SharedApp& get();
    SharedApp& operator=(const SharedApp&) = delete;
    SharedApp(const SharedApp&) = delete;

    /**
     * @brief getRecentFiles Gets a reference to the recent files list.
     * @return The recent files list.
     */
    RecentItems& getRecentFiles();

    /**
     * @brief getJobLookup Gets the shared job lookup object, for finding existing jobs.
     * @return The job lookup object.
     */
    job::JobLookup& getJobLookup();

    /**
     * @brief getImageLookup Gets the shared image lookup object, for finding existing image objects.
     * @return The image lookup object.
     */
    image::ImageLookup& getImageLookup();

    /**
     * @brief getGlobalPreferences Gets the shared global preferences object, which stores global application preferences.
     * @return The global preferences object.
     */
    preferences::GlobalPreferences& getGlobalPreferences();

private:
    class SharedAppImpl;
    std::unique_ptr<SharedAppImpl> d;

    SharedApp();
    ~SharedApp();
};

}

}

}
