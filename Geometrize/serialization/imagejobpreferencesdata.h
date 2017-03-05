#pragma once

#include "cereal/cereal.hpp"
#include "cereal/types/vector.hpp"

#include "geometrize/shape/shapetypes.h"

namespace geometrize
{

namespace serialization
{

/**
 * @brief The ImageJobPreferencesData struct represents the preferences data for an image job.
 */
class ImageJobPreferencesData
{
public:
    template<class Archive>
    void save(Archive& archive)
    {
        archive(CEREAL_NVP(shapes));
    }

    template<class Archive>
    void load(Archive& archive)
    {
        archive(CEREAL_NVP(shapes));
    }

private:
    std::vector<geometrize::shapes::ShapeTypes> shapes;
};

}

}
