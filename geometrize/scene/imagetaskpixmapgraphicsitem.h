#pragma once

#include <QEvent>
#include <QGraphicsPixmapItem>
#include <QFocusEvent>
#include <QKeyEvent>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QObject>
#include <QPixmap>

#include "customtabletevent.h"

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
    void signal_onCustomTabletEvent(const geometrize::scene::CustomTabletEvent& event);
    void signal_onHoverMoveEvent(double lastX, double lastY, double x, double y, bool ctrlModifier);
    void signal_onMousePressEvent(double x, double y, bool ctrlModifier);
    void signal_onMouseMoveEvent(double lastX, double lastY, double x, double y, bool ctrlModifier);
    void signal_onMouseReleaseEvent(double x, double y, bool ctrlModifier);
    void signal_onWheelEvent(double x, double y, int amount, bool ctrlModifier);
    void signal_onKeyPressEvent(int key, bool ctrlModifier);
    void signal_onKeyReleaseEvent(int key, bool ctrlModifier);
    void signal_onFocusInEvent();
    void signal_onFocusOutEvent();

protected:
    bool sceneEvent(QEvent* event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void wheelEvent(QGraphicsSceneWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
};

}

}
