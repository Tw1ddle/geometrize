#pragma once

#include "cereal/cereal.hpp"

namespace geometrize
{

namespace serialization
{

/**
 * @brief The ImageJobPreferencesData struct represents the preferences data for an image job.
 */
struct ImageJobPreferencesData
{
    template<class Archive>
    void save(Archive& archive)
    {
        //archive();
    }

    template<class Archive>
    void load(Archive& archive)
    {
        //archive();
    }
};

}

}
