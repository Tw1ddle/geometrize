#pragma once

#include <memory>
#include <string>
#include <vector>

namespace geometrize
{

/**
 * @brief The TemplateManifest class represents the metadata for a job template.
 */
class TemplateManifest
{
public:
    TemplateManifest(const std::string& manifestFilepath);
    TemplateManifest& operator=(const TemplateManifest&) = delete;
    TemplateManifest(const TemplateManifest&) = delete;
    ~TemplateManifest();

    std::string getName() const;
    std::vector<std::string> getTags() const;
    std::vector<std::string> getAuthors() const;
    std::string getLicense() const;

private:
    class TemplateManifestImpl;
    std::unique_ptr<TemplateManifestImpl> d;
};

}
