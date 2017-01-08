#include "templatemanifest.h"

#include <fstream>

namespace geometrize
{

TemplateManifest::TemplateManifest(const std::string& manifestFilepath)
{
    std::ifstream input(manifestFilepath);

    try {
        m_data.load(cereal::JSONInputArchive(input));
    } catch(...) {
        assert(0 && "Failed to read template manifest");
    }
}

std::string TemplateManifest::getName() const
{
    return m_data.name;
}

std::vector<std::string> TemplateManifest::getTags() const
{
    return m_data.tags;
}

std::vector<std::string> TemplateManifest::getAuthors() const
{
    return m_data.authors;
}

std::string TemplateManifest::getLicense() const
{
    return m_data.license;
}

}
