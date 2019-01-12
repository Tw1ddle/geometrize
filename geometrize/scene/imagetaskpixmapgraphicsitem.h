#pragma once

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QObject>
#include <QPixmap>

namespace geometrize
{

namespace scene
{

/**
 * @brief The ImageTaskPixmapGraphicsItem class models a pixmap graphic item that goes into the scene for an image task.
 * This usually represents a pixmap of the working image that is being transformed into shapes.
 */
class ImageTaskPixmapGraphicsItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    ImageTaskPixmapGraphicsItem();
    explicit ImageTaskPixmapGraphicsItem(const QPixmap& pixmap);
    virtual ~ImageTaskPixmapGraphicsItem();

signals:
    void signal_onHoverMoveEvent(double lastX, double lastY, double x, double y, bool ctrlModifier);
    void signal_onMousePressEvent(double x, double y, bool ctrlModifier);
    void signal_onMouseMoveEvent(double lastX, double lastY, double x, double y, bool ctrlModifier);
    void signal_onMouseReleaseEvent(double x, double y, bool ctrlModifier);
    void signal_onWheelEvent(double x, double y, int amount, bool ctrlModifier);

protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void wheelEvent(QGraphicsSceneWheelEvent* event) override;
};

}

}
