#include "templatemanifest.h"

#include <cassert>
#include <istream>

#include "cereal/archives/json.hpp"

#include "common/util.h"
#include "serialization/streamview.h"
#include "serialization/templatemetadata.h"

namespace geometrize
{

class TemplateManifest::TemplateManifestImpl
{
public:
    TemplateManifestImpl()
    {
    }

    TemplateManifestImpl(const std::string& manifestFilepath)
    {
        // Templates can be bundled into Qt resources, so we use a streamview that loads the file contents into a byte array first
        serialization::StreamView streamView(manifestFilepath);
        std::istream input(&streamView);
        try {
            cereal::JSONInputArchive archive{input};
            m_data.load(archive);
        } catch(...) {
            assert(0 && "Failed to read template manifest");
        }
    }
    ~TemplateManifestImpl() = default;
    TemplateManifestImpl& operator=(const TemplateManifestImpl&) = default;
    TemplateManifestImpl(const TemplateManifestImpl&) = default;

    serialization::TemplateMetadata m_data;
};

TemplateManifest::TemplateManifest() : d{std::make_shared<TemplateManifest::TemplateManifestImpl>()}
{
}

TemplateManifest::TemplateManifest(const std::string& manifestFilepath) : d{std::make_shared<TemplateManifest::TemplateManifestImpl>(manifestFilepath)}
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
