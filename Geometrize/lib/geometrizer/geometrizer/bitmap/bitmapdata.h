#ifndef BITMAPDATA_H
#define BITMAPDATA_H

#include <cstdint>
#include <vector>

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
     */
    inline BitmapData(const std::size_t width, const std::size_t height) : m_width{width}, m_height{height}, m_data(width * height) {}

    /**
     * @brief BitmapData Creates a new bitmap from the supplied byte data.
     * @param width The width of the bitmap.
     * @param height The height of the bitmap.
     * @param data The byte data to fill the bitmap with, must be width * height * depth (4) long.
     */
    inline BitmapData(const std::size_t width, const std::size_t height, const std::vector<unsigned char>& data) : m_width{width}, m_height{height}, m_data{data} {}

    ~BitmapData() = default;
    BitmapData& operator=(const BitmapData&) = delete;
    BitmapData(const BitmapData&) = delete;

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

    // TODO get the rgb components, or work on the data directly?
    inline unsigned char getPixel(const int x, const int y) const
    {
        const std::size_t index{m_width * y + x};
        return m_data[index];
    }

    inline void setPixel(const int x, const int y, const unsigned char color)
    {
        const std::size_t index{m_width * y + x};
        m_data[index] = color;
    }

private:
    const std::size_t m_width; ///< The width of the bitmap.
    const std::size_t m_height; ///< The height of the bitmap.
    std::vector<unsigned char> m_data; ///< The bitmap data.
};

}

#endif // BITMAPDATA_H
