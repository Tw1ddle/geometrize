#include "joblookup.h"

#include "imagejob.h"

#include <map>

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
        return m_jobs[id];
    }

    void setImageJob(const std::string& id, ImageJob* job)
    {
        m_jobs[id] = job;
    }

private:
    std::map<std::string, ImageJob*> m_jobs;
};

ImageJob* JobLookup::getImageJob(const std::string& id)
{
    return d->getImageJob(id);
}

void JobLookup::setImageJob(const std::string& id, ImageJob* job)
{
    d->setImageJob(id, job);
}

JobLookup::JobLookup() : d{std::make_unique<JobLookup::JobLookupImpl>()}
{

}

JobLookup::~JobLookup()
{

}

}

}
