#ifndef UTIL_H
#define UTIL_H

#include <algorithm>

namespace geometrize
{

namespace util
{

/**
 * Clamps a value within a range.
 * @param value The value to clamp.
 * @param min The lower bound of the range.
 * @param max The upper bound of the range.
 * @return The clamped value.
 */
template<typename T> T clamp(const T& value, const T& lower, const T& upper)
{
    return std::max(lower, std::min(value, upper));
}

}

}

#endif // UTIL_H
