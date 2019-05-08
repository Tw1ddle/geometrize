#pragma once

#include <memory>

#include <QGraphicsScene>

#include "imagetaskpixmapgraphicsitem.h"
#include "svgitem.h"

class QFocusEvent;
class QKeyEvent;
class QGraphicsSceneWheelEvent;

namespace geometrize
{

namespace scene
{

/**
 * @brief The ImageTaskScene class models a graphics scene for viewing the results of image tasks.
 */
class ImageTaskScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit ImageTaskScene(QObject* parent = nullptr);
    virtual ~ImageTaskScene();

    /**
     * @brief setTargetPixmapOpacity Sets the opacity of the target pixmap.
     * @param opacity The opacity value.
     */
    void setTargetPixmapOpacity(float opacity);

    /**
     * @brief setTargetPixmap Sets the pixmap that provides the target/goal image visualization.
     * @param pixmap The pixmap to set as the target/goal image visualization.
     */
    void setTargetPixmap(const QPixmap& pixmap);

    /**
      * @brief getTargetPixmapItem Gets a refernece to the graphics item that houses the target pixmap.
      * @return The target pixmap item.
      */
     ImageTaskPixmapGraphicsItem& getTargetPixmapItem();

     /**
      * @brief setOverlayItem Sets the given overlay shape upon the scene.
      * @param item The item to add.
      */
     void setOverlayShape(SvgItem* item);

protected:
     void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
     void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
     void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
     void wheelEvent(QGraphicsSceneWheelEvent* event) override;
     void keyPressEvent(QKeyEvent* event) override;
     void keyReleaseEvent(QKeyEvent* event) override;
     void focusInEvent(QFocusEvent* event) override;
     void focusOutEvent(QFocusEvent* event) override;

private:
    class ImageTaskSceneImpl;
    std::unique_ptr<ImageTaskSceneImpl> d;
};

}

}
