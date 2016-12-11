#pragma once

#include <algorithm>
#include <vector>

#include "bitmap/bitmapdata.h"
#include "shape/shapetypes.h"
#include "util.h"

namespace geometrize
{

/**
 * @brief The Scanline class represents a scanline, a row of pixels running across a bitmap.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class Scanline
{
public:
    /**
     * @brief Scanline Creates a new scanline.
     * @param y The y-coordinate.
     * @param x1 The leftmost x-coordinate.
     * @param x2 The rightmost x-coordinate.
     * @param alpha The alpha transparency of the scanline.
     */
    inline Scanline(const int y, const int x1, const int x2, const int alpha) : y{y}, x1{x1}, x2{x2}, alpha{alpha} {}

    ~Scanline() = default;
    Scanline& operator=(const Scanline&) = default;
    Scanline(const Scanline&) = default;

    // TODO modify the scanlines in-place?

    /**
     * @brief trim Crops the scanning width of an array of scanlines so they do not scan outside of the given area.
     * @param scanlines The scanlines to crop.
     * @param w The width to crop.
     * @param h The height to crop.
     * @return A new vector of cropped scanlines.
     */
    static inline std::vector<Scanline> trim(std::vector<Scanline>& scanlines, const int w, const int h)
    {
        std::vector<Scanline> trimmedScanlines;

        for(auto& line : scanlines) {
            if(line.y < 0 || line.y >= h || line.x1 > w || line.x2 < 0) {
                continue;
            }
            line.x1 = geometrize::util::clamp(line.x1, 0, w - 1);
            line.x2 = geometrize::util::clamp(line.x2, 0, w - 1);
            if(line.x1 > line.x2) {
                continue;
            }
            trimmedScanlines.push_back(line);
        }

        return trimmedScanlines;
    }

    const int y; ///< The y-coordinate of the scanline.
    int x1; ///< The leftmost x-coordinate of the scanline.
    int x2; ///< The rightmost x-coordinate of the scanline.
    const int alpha; ///< The alpha/opacity of the scanline.
};

}
