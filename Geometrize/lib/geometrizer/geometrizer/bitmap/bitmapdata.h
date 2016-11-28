#ifndef BITMAPDATA_H
#define BITMAPDATA_H

#include <cstdint>
#include <vector>

#include "rgba.h"

namespace geometrize
{

/**
 * Helper class for working with bitmap data.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class BitmapData
{
public:
    /**
     * @brief BitmapData Creates a new bitmap.
     * @param width The width of the bitmap.
     * @param height The height of the bitmap.
     * @param color The starting color of the bitmap (RGBA format).
     */
    inline BitmapData(const std::size_t width, const std::size_t height, const rgba color) : m_width{width}, m_height{height}, m_data(width * height * 4)
    {
        if(color.r == 0 && color.g == 0 && color.b == 0 && color.a == 0) {
            return;
        }

        for(int i = 0; i < m_data.size(); i+=4) {
            m_data[i] = color.r;
            m_data[i + 1] = color.g;
            m_data[i + 2] = color.b;
            m_data[i + 3] = color.a;
        }
    }

    /**
     * @brief BitmapData Creates a new bitmap from the supplied byte data.
     * @param width The width of the bitmap.
     * @param height The height of the bitmap.
     * @param data The byte data to fill the bitmap with, must be width * height * depth (4) long.
     */
    inline BitmapData(const std::size_t width, const std::size_t height, const std::vector<unsigned char>& data) : m_width{width}, m_height{height}, m_data{data} {}

    ~BitmapData() = default;
    BitmapData& operator=(const BitmapData&) = default;
    BitmapData(const BitmapData&) = default;

    /**
     * @brief getWidth Gets the width of the bitmap.
     */
    inline std::size_t getWidth() const
    {
        return m_width;
    }

    /**
     * @brief getHeight Gets the height of the bitmap.
     */
    inline std::size_t getHeight() const
    {
        return m_height;
    }

    /**
     * @brief copyData Gets a copy of the raw bitmap data.
     * @return	The bitmap data.
     */
    inline std::vector<unsigned char> copyData() const
    {
        return m_data;
    }

    /**
     * @brief getDataRef Gets a reference to the raw bitmap data.
     * @return  The bitmap data.
     */
    inline const std::vector<unsigned char>& getDataRef()
    {
        return m_data;
    }

    /**
     * @brief getPixel Gets a pixel color value.
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @return The pixel RGBA color value.
     */
    inline rgba getPixel(const int x, const int y) const
    {
        const std::size_t index{m_width * y + x};
        return rgba{m_data[index], m_data[index + 1], m_data[index + 2], m_data[index + 3]};
    }

    /**
     * @brief setPixel Sets a pixel color value.
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @param color The pixel RGBA color value.
     */
    inline void setPixel(const int x, const int y, const rgba color)
    {
        const std::size_t index{m_width * y + x};
        m_data[index] = color.r;
        m_data[index + 1] = color.g;
        m_data[index + 2] = color.b;
        m_data[index + 3] = color.a;
    }

private:
    const std::size_t m_width; ///< The width of the bitmap.
    const std::size_t m_height; ///< The height of the bitmap.
    std::vector<unsigned char> m_data; ///< The bitmap data.
};

}

#endif // BITMAPDATA_H
