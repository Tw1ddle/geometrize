#pragma once

#include <memory>
#include <string>

namespace geometrize
{

namespace job
{
class ImageJob;
}

}

namespace geometrize
{

namespace job
{

/**
 * @brief The JobLookup class provides a mechanism for storing and retrieving job objects.
 */
class JobLookup
{
public:
    JobLookup();
    JobLookup& operator=(const JobLookup&) = delete;
    JobLookup(const JobLookup&) = delete;
    ~JobLookup();

    // TODO setup proper ids, paths are insufficient since there can be more than one
    ImageJob* getImageJob(const std::string& id);
    void setImageJob(const std::string& id, ImageJob* job);

private:
    class JobLookupImpl;
    std::unique_ptr<JobLookupImpl> d;
};

}

}
