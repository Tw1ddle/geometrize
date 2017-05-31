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

    std::shared_ptr<ImageJob> getImageJob(const std::string& id)
    {
        return m_jobs[id];
    }

    void setImageJob(const std::string& id, const std::shared_ptr<ImageJob>& job)
    {
        m_jobs[id] = job;
    }

    void removeImageJob(const std::string& id)
    {
        m_jobs.erase(id);
    }

private:
    std::map<std::string, std::shared_ptr<ImageJob>> m_jobs;
};

std::shared_ptr<ImageJob> JobLookup::getImageJob(const std::string& id)
{
    return d->getImageJob(id);
}

void JobLookup::setImageJob(const std::string& id, const std::shared_ptr<ImageJob>& job)
{
    d->setImageJob(id, job);
}

void JobLookup::removeImageJob(const std::string& id)
{
    d->removeImageJob(id);
}

JobLookup::JobLookup() : d{std::make_unique<JobLookup::JobLookupImpl>()}
{

}

JobLookup::~JobLookup()
{

}

}

}
