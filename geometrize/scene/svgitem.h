#pragma once

#include <QByteArray>
#include <QKeyEvent>
#include <QFocusEvent>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsSvgItem>

namespace geometrize
{

namespace scene
{

/**
 * @brief The SvgItem class represents an SVG graphic that is drawn as part of a scene.
 */
class SvgItem : public QGraphicsSvgItem
{
    Q_OBJECT
public:
    explicit SvgItem(const QByteArray& data);
    virtual ~SvgItem();

signals:
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
