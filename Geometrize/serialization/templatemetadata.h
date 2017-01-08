#pragma once

#include <string>
#include <vector>

#include "cereal/types/vector.hpp"
#include "cereal/cereal.hpp"

namespace geometrize
{

namespace serialization
{

/**
 * @brief The TemplateMetadata struct represents the metadata (such as license information) for a job template.
 */
struct TemplateMetadata
{
    std::string name; ///< The name of the template, suitable for displaying to the user.
    std::vector<std::string> tags; ///< Tag-strings which could be used by the application to sort or search for templates.
    std::vector<std::string> authors; ///< User names which identify the authors of the template.
    std::string license; ///< The license under which the template (and associated assets) is released e.g. MIT, or explanatory text regarding licensing.

    template<class Archive>
    void save(Archive& archive)
    {
        archive(CEREAL_NVP(name), CEREAL_NVP(tags), CEREAL_NVP(authors), CEREAL_NVP(license));
    }

    template<class Archive>
    void load(Archive& archive)
    {
        archive(CEREAL_NVP(name), CEREAL_NVP(tags), CEREAL_NVP(authors), CEREAL_NVP(license));
    }
};

}

}
