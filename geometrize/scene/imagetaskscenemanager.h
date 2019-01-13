#pragma once

#include <memory>
#include <vector>

#include <QObject>

class QPixmap;

namespace geometrize
{
struct ShapeResult;
class Shape;
}

namespace geometrize
{
namespace scene
{
class ImageTaskGraphicsView;
}
}

namespace geometrize
{

namespace scene
{

/**
 * @brief The ImageTaskSceneManager manages the scenes representing the graphical state of the shapes
 */
class ImageTaskSceneManager : public QObject
{
    Q_OBJECT

public:
    ImageTaskSceneManager();
    virtual ~ImageTaskSceneManager();
	
	/**
	 * @brief setViews Associates the given views with the scenes managed by this class.
	 * @param pixmapView The pixmap view that will display the pixmap-based view.
	 * @param svgView The svg-based view that will display the vector graphics-based SVG view.
	 */
    void setViews(ImageTaskGraphicsView& pixmapView, ImageTaskGraphicsView& svgView);

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
     * @brief updateScenes Updates the scenes to show the latest image/shapes.
     * @param pixmap The target pixmap.
     * @param shapes The shapes added to the scene.
     */
    void updateScenes(const QPixmap& pixmap, const std::vector<geometrize::ShapeResult>& shapes);
	
	/**
	 * @brief reset Resets the state of the scenes, removing shapes etc.
	 */
	void reset();
	
    /**
     * @brief fitPixmapSceneInView Fits the pixmap scene into the given view.
     * @param pixmapView The view to fit the scene into.
     */
	void fitPixmapSceneInView(ImageTaskGraphicsView& pixmapView);

    /**
     * @brief fitVectorSceneInView Fits the SVG/vector scene into the given view.
     * @param svgView The view to fit the scene into.
     */
	void fitVectorSceneInView(ImageTaskGraphicsView& svgView);

    /**
     * @brief fitScenesInViews Fits the scenes into their respective views.
     * @param pixmapView The pixmap-based view.
     * @param svgView The vector/SVG-based view.
     */
	void fitScenesInViews(ImageTaskGraphicsView& pixmapView, ImageTaskGraphicsView& svgView);

    /**
     * @brief setAreaOfInfluenceShape Sets up a area of influence visual for the given shape.
     * @param shape The area of influence shape.
     */
    void setAreaOfInfluenceShape(const geometrize::Shape& shape);

signals:
    void signal_onTargetImageHoverMoveEvent(double lastX, double lastY, double x, double y, bool ctrlModifier);
    void signal_onTargetImageMousePressEvent(double x, double y, bool ctrlModifier);
    void signal_onTargetImageMouseMoveEvent(double lastX, double lastY, double x, double y, bool ctrlModifier);
    void signal_onTargetImageMouseReleaseEvent(double x, double y, bool ctrlModifier);
    void signal_onTargetImageWheelEvent(double x, double y, int amount, bool ctrlModifier);

    void signal_onAreaOfInfluenceShapeMouseWheelEvent(double x, double y, int amount, bool ctrlModifier);

private:
    class ImageTaskSceneManagerImpl;
    std::unique_ptr<ImageTaskSceneManagerImpl> d;
};

}

}
