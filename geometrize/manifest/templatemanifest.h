#pragma once

#include <memory>
#include <string>
#include <vector>

namespace geometrize
{

/**
 * @brief The TemplateManifest class represents the metadata for a task template.
 */
class TemplateManifest
{
public:
    TemplateManifest(const std::string& manifestFilepath);
    TemplateManifest& operator=(const TemplateManifest&) = default;
    TemplateManifest(const TemplateManifest&) = default;
    ~TemplateManifest();

    std::string getName() const;
    std::vector<std::string> getTags() const;
    std::vector<std::string> getAuthors() const;
    std::string getLicense() const;

private:
    class TemplateManifestImpl;
    std::shared_ptr<TemplateManifestImpl> d;
};

}
