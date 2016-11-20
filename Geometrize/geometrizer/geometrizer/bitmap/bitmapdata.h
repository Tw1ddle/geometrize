#ifndef BITMAPDATA_H
#define BITMAPDATA_H

#include <cstdint>
#include <vector>

/**
 * Helper class for working with bitmap data.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class BitmapData
{
public:
    inline BitmapData(std::size_t width, std::size_t height) : m_width{width}, m_height{height}, m_data{width * height}
    {

    }

    ~BitmapData() = default;
    BitmapData& operator=(const BitmapData&) = delete;
    BitmapData(const BitmapData&) = delete;

    /**
     * @brief getWidth Gets the width of the bitmap.
     */
    inline auto getWidth() -> decltype(m_width)
    {
        return m_width;
    }

    /**
     * @brief getHeight Gets the height of the bitmap.
     */
    inline auto getHeight() -> decltype(m_height)
    {
        return m_height;
    }

private:
    std::size_t m_width; ///< The width of the bitmap.
    std::size_t m_height; ///< The height of the bitmap.
    std::vector<std::uint8_t> m_data; ///< The bitmap data.
};

#endif // BITMAPDATA_H
