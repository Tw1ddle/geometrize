#pragma once

#include <map>
#include <string>

#include "cereal/cereal.hpp"
#include "cereal/types/map.hpp"
#include "cereal/types/vector.hpp"

#include "geometrize/runner/imagerunneroptions.h"

namespace geometrize
{

template<typename Archive>
void serialize(Archive& ar, geometrize::ImageRunnerShapeBoundsOptions& bounds)
{
    ar(cereal::make_nvp("enabled", bounds.enabled),
       cereal::make_nvp("xMinPercent", bounds.xMinPercent),
       cereal::make_nvp("yMinPercent", bounds.yMinPercent),
       cereal::make_nvp("xMaxPercent", bounds.xMaxPercent),
       cereal::make_nvp("yMaxPercent", bounds.yMaxPercent));
}

namespace serialization
{

/**
 * @brief The ImageTaskPreferencesData struct represents the preferences data for an image task.
 */
class ImageTaskPreferencesData
{
public:
    template<class Archive>
    void archive(Archive& ar, geometrize::ImageRunnerOptions& options, bool& scriptsEnabled, std::map<std::string, std::string>& scripts)
    {
        ar(cereal::make_nvp(shapeAlphaKey, options.alpha));
        ar(cereal::make_nvp(maxShapeMutationsKey, options.maxShapeMutations));
        ar(cereal::make_nvp(shapeCountKey, options.shapeCount));
        ar(cereal::make_nvp(shapeTypesKey, options.shapeTypes));
        ar(cereal::make_nvp(randomSeedKey, options.seed));
        ar(cereal::make_nvp(maxThreadsKey, options.maxThreads));
        ar(cereal::make_nvp(shapeBounds, options.shapeBounds));
        ar(cereal::make_nvp(scriptsEnabledKey, scriptsEnabled));
        ar(cereal::make_nvp(scriptsKey, scripts));
    }

private:
    const std::string shapeAlphaKey{"shapeAlpha"};
    const std::string maxShapeMutationsKey{"maxShapeMutations"};
    const std::string shapeCountKey{"shapeCount"};
    const std::string shapeTypesKey{"shapeTypes"};
    const std::string randomSeedKey{"randomSeed"};
    const std::string maxThreadsKey{"maxThreads"};
    const std::string shapeBounds{"shapeBounds"};

    const std::string scriptsEnabledKey{"scriptModeEnabled"};
    const std::string scriptsKey{"scripts"};
};

}

}
