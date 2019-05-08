#pragma once

#include "svgitem.h"

#include <QByteArray>

class QGraphicsSceneWheelEvent;


namespace geometrize
{

namespace scene
{

/**
 * @brief The AreaOfInfluenceShapeItem class represents a graphical visualization of a shape that the user interacts with
 * to control where shapes are spawned/mutated as an image is geometrized.
 */
class AreaOfInfluenceShapeItem : public SvgItem
{
    Q_OBJECT
public:
    explicit AreaOfInfluenceShapeItem(const QByteArray& data);
    virtual ~AreaOfInfluenceShapeItem();

protected:
    void wheelEvent(QGraphicsSceneWheelEvent* event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
};

}

}
