#pragma once

#include <string>
#include <vector>

#include "cereal/cereal.hpp"
#include "cereal/types/vector.hpp"

namespace geometrize
{

namespace serialization
{

/**
 * @brief The StringVectorData struct represents a vector of strings (as might be used for a history of terminal input, for example).
 */
struct StringVectorData
{
    std::vector<std::string> strings; ///< The vector of strings.

    template<class Archive>
    void save(Archive& archive)
    {
        archive(CEREAL_NVP(strings));
    }

    template<class Archive>
    void load(Archive& archive)
    {
        archive(strings);
    }
};

}

}
