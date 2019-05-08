#pragma once

#include <utility>
#include <vector>

#include <QObject>

#include "geometrize/shape/shape.h"
#include "geometrize/shape/shapetypes.h"

#include "scene/imagetaskscenemanager.h"

namespace geometrize
{

namespace scene
{

namespace tools
{

/**
 * @brief The AreaOfInfluenceShapes class represents a collection of shapes which are used to
 * influence where other shapes that form a geometrized image are spawned and/or mutated. These shapes are
 * displayed in an image task's scenes, which the user interacts with to manipulate areas of influence.
 */
class AreaOfInfluenceShapes : public QObject
{
    Q_OBJECT
public:
    AreaOfInfluenceShapes();
    ~AreaOfInfluenceShapes() = default;

    /**
     * @brief connectToSceneManager Connects the scene manager to the area of influence shapes.
     * This is so that user interaction with the scene can be used to manipulate the shapes.
     */
    void connectToSceneManager(const geometrize::scene::ImageTaskSceneManager& mgr);

    /**
     * @brief setup Sets the current area of influence shape to the given shape.
     * @param shape The shape.
     */
    void setup(const geometrize::Shape& shape);

    /**
     * @brief selectNextShape Selects the next area of influence shape to use.
     */
    void selectNextShape();

    /**
     * @brief translateShape Translates the current area of influence shape by the given number of pixels.
     * @param x Number of pixels to translate by on the x-axis.
     * @param y Number of pixels to translate by on the y-axis.
     */
    void translateShape(float x, float y);

    /**
     * @brief scaleShape Scales the current shape by the given factor.
     * @param scaleFactor The scale factor to scale the shape by.
     */
    void scaleShape(float scaleFactor);

    /**
     * @brief rotateShape Rotates the current shape by the given angle.
     * @param angle The angle to rotate the shape by.
     */
    void rotateShape(float angle);

    /**
     * @brief getCurrentShape Gets the most recently used shape.
     * @return The most recently used shape.
     */
    std::shared_ptr<geometrize::Shape> getCurrentShape() const;

    /**
     * @brief getPixels Gets the pixels that make up the current area of influence shape.
     * @param xBound The max x-value allowed in the pixels.
     * @param yBound The max y-value allowed in the pixels.
     * @return A vector of x,y pairs that make up the pixels of the shape. Empty if there is no shape.
     */
    std::vector<std::pair<std::int32_t, std::int32_t>> getPixels(std::uint32_t xBound, std::uint32_t yBound) const;

signals:
    /**
     * @brief signal_didModifyShape Signal dispatched whenever the current area of influence shape is modified.
     * @param shape Geometrize's representation of the shape.
     */
    void signal_didModifyShape(const geometrize::Shape& shape);

private:
    std::size_t m_currentShapeId; ///> Index of the current shape in use.
    std::vector<std::shared_ptr<geometrize::Shape>> m_shapes; ///> The area of influence shapes in use.
};

}

}

}
