#pragma once

#include <QObject>

#include "geometrize/shape/shape.h"

namespace geometrize
{

namespace scene
{

/**
 * @brief The AreaOfInfluenceShape class represents a shape, which is used to
 * influence how other shapes that form a geometrized image are spawned or mutated, and is
 * rendered atop an image task scene, which a user interacts with to manipulate the shape.
 */
class AreaOfInfluenceShape : public QObject
{
    Q_OBJECT
public:
    AreaOfInfluenceShape() = default;
    ~AreaOfInfluenceShape() = default;

    /**
     * @brief setup Sets up the default area of influence shape, a small circle.
     * @param x The center x-coordinate of the shape, in target image pixel coordinates.
     * @param y The center y-coordinate of the shape, in target image pixel coordinates.
     * @param radius The radius of the circle in pixels.
     */
    void setup(int x, int y, unsigned int radius);

    /**
     * @brief setup Sets up the area of influence shape to the given shape.
     * @param shape The shape.
     */
    void setup(const geometrize::Shape& shape);

    /**
     * @brief translateShape Translates the area of influence shape by the given number of pixels.
     * @param x Number of pixels to translate by on the x-axis.
     * @param y Number of pixels to translate by on the y-axis.
     */
    void translateShape(float x, float y);

    /**
     * @brief scaleShape Scales the shape by the given factor.
     * @param scaleFactor The scale factor to scale the shape by.
     */
    void scaleShape(float scaleFactor);

signals:
    /**
     * @brief signal_didModifyShape Signal dispatched whenever the area of influence shape is modified.
     * @param shape The geometrize-based representation of the shape.
     */
    void signal_didModifyShape(const geometrize::Shape& shape);

private:
    std::shared_ptr<geometrize::Shape> m_lastShape; ///> The most recently used shape.
};

}

}
