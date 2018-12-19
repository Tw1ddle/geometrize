#pragma once

#include <memory>
#include <vector>

#include <QGraphicsScene>

namespace geometrize
{
struct ShapeResult;
}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The ImageTaskSvgScene class models a graphics scene for viewing the shape data of an image task as an SVG.
 */
class ImageTaskSvgScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit ImageTaskSvgScene();
    virtual ~ImageTaskSvgScene();

    /**
     * @brief setTargetPixmap Sets the pixmap that provides the target/goal image visualization.
     * @param pixmap The pixmap to set as the target/goal image visualization.
     */
    void setTargetPixmap(const QPixmap& pixmap);

    /**
     * @brief setTargetPixmapOpacity Sets the opacity of the target pixmap.
     * @param opacity The opacity value.
     */
    void setTargetPixmapOpacity(float opacity);

    /**
     * @brief drawSvg Draws the SVG data based on the given shapes.
     * @param width The width of the SVG.
     * @param height The height of the SVG.
     * @param shapes The shapes to draw in the scene.
     */
    void drawSvg(const std::vector<geometrize::ShapeResult>& shapes, const std::uint32_t width, const std::uint32_t height);

private:
    class ImageTaskSvgSceneImpl;
    std::unique_ptr<ImageTaskSvgSceneImpl> d;
};

}

}
