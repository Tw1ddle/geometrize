#pragma once

#include "cereal/cereal.hpp"

namespace geometrize
{

namespace serialization
{

/**
 * @brief The GlobalPreferencesData struct represents the data for the global application preferences.
 */
struct GlobalPreferencesData
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
