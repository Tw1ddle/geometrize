#pragma once

#include <memory>
#include <vector>

#include <QGraphicsScene>

#include "imagetaskscene.h"

namespace geometrize
{
struct ShapeResult;
}

namespace geometrize
{

namespace scene
{

/**
 * @brief The ImageTaskSvgScene class models a graphics scene for viewing the shape data of an image task as an SVG.
 */
class ImageTaskSvgScene : public ImageTaskScene
{
    Q_OBJECT

public:
    ImageTaskSvgScene();
    virtual ~ImageTaskSvgScene();

    /**
     * @brief addShapes Updates the SVG data based on the given shapes.
     * @param shapes The shapes to draw in the scene.
     * @param width The width of the SVG.
     * @param height The height of the SVG.
     */
    void addShapes(const std::vector<geometrize::ShapeResult>& shapes, const std::uint32_t width, const std::uint32_t height);

    /**
     * @brief removeShapes Removes all shapes from the scene.
     */
    void removeShapes();

private:
    class ImageTaskSvgSceneImpl;
    std::unique_ptr<ImageTaskSvgSceneImpl> d;
};

}

}
