#pragma once

#include <string>

#include "cereal/cereal.hpp"
#include "cereal/types/vector.hpp"

#include "geometrize/runner/imagerunneroptions.h"

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
    void archive(Archive& ar, geometrize::ImageRunnerOptions& options)
    {
        ar(cereal::make_nvp(shapeAlphaKey, options.alpha));
        ar(cereal::make_nvp(maxShapeMutationsKey, options.maxShapeMutations));
        ar(cereal::make_nvp(shapeCountKey, options.shapeCount));
        ar(cereal::make_nvp(shapeTypesKey, options.shapeTypes));
    }

private:
    const std::string shapeAlphaKey{"shapeAlpha"};
    const std::string maxShapeMutationsKey{"maxShapeMutations"};
    const std::string shapeCountKey{"shapeCount"};
    const std::string shapeTypesKey{"shapeTypes"};
};

}

}
