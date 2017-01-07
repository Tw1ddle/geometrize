#pragma once

#include <string>
#include <vector>

namespace geometrize
{

namespace serialization
{

/**
 * @brief The TemplateMetadata struct represents the metadata (such as license information) for a job template.
 */
struct TemplateMetadata
{
    std::string m_name; ///< The name of the template, suitable for displaying to the user.
    std::vector<std::string> m_tags; ///< Tag-strings which could be used by the application to sort or search for templates.
    std::vector<std::string> m_authors; ///< User names which identify the authors of the template.
    std::string m_license; ///< The license under which the template (and associated assets) is released e.g. MIT, or explanatory text regarding licensing.
};

}

}
