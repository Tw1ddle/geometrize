#include "joblookup.h"

#include "imagejob.h"

namespace geometrize
{

namespace job
{

class JobLookup::JobLookupImpl
{
public:
    JobLookupImpl() = default;
    JobLookupImpl& operator=(const JobLookupImpl&) = delete;
    JobLookupImpl(const JobLookupImpl&) = delete;
    ~JobLookupImpl() = default;

    ImageJob* getImageJob(const std::string& id)
    {
        return nullptr; // TODO
    }

    void setImageJob(const std::string& id)
    {
        // TODO
    }

private:
};

ImageJob* JobLookup::getImageJob(const std::string& id)
{
    return d->getImageJob(id);
}

void JobLookup::setImageJob(const std::string& id)
{
    d->setImageJob(id);
}

JobLookup::JobLookup() : d{std::make_unique<JobLookup::JobLookupImpl>()}
{

}

JobLookup::~JobLookup()
{

}

}

}
