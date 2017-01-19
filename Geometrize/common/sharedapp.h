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

private:
    class SharedAppImpl;
    std::unique_ptr<SharedAppImpl> d;

    SharedApp();
    ~SharedApp();
};

}

}

}
