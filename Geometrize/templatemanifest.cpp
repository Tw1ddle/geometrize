#include "templatemanifest.h"

#include <assert.h>
#include <fstream>

#include "serialization/templatemetadata.h"
#include "cereal/archives/json.hpp"

namespace geometrize
{

class TemplateManifest::TemplateManifestImpl
{
public:
    TemplateManifestImpl(const std::string& manifestFilepath)
    {
        std::ifstream input(manifestFilepath);
        try {
            m_data.load(cereal::JSONInputArchive(input));
        } catch(...) {
            assert(0 && "Failed to read template manifest");
        }
    }
    ~TemplateManifestImpl() = default;
    TemplateManifestImpl& operator=(const TemplateManifestImpl&) = default;
    TemplateManifestImpl(const TemplateManifestImpl&) = default;

    serialization::TemplateMetadata m_data;
};

TemplateManifest::TemplateManifest(const std::string& manifestFilepath) : d{std::make_unique<TemplateManifest::TemplateManifestImpl>(manifestFilepath)}
{
}

TemplateManifest::~TemplateManifest()
{
}

std::string TemplateManifest::getName() const
{
    return d->m_data.name;
}

std::vector<std::string> TemplateManifest::getTags() const
{
    return d->m_data.tags;
}

std::vector<std::string> TemplateManifest::getAuthors() const
{
    return d->m_data.authors;
}

std::string TemplateManifest::getLicense() const
{
    return d->m_data.license;
}

}
