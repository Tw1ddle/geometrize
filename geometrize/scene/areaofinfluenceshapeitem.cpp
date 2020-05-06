#include "areaofinfluenceshapeitem.h"

#include <QByteArray>
#include <QGraphicsSceneWheelEvent>

namespace geometrize
{

namespace scene
{

AreaOfInfluenceShapeItem::AreaOfInfluenceShapeItem(const QByteArray& data) : SvgItem{data}
{
    setAcceptDrops(false);
    setAcceptHoverEvents(false);
    setAcceptTouchEvents(false);
    setFlag(ItemIsFocusable, false);
    setFlag(QGraphicsItem::ItemClipsToShape, true);
    setCacheMode(QGraphicsItem::NoCache);
    setEnabled(false);
    setZValue(100); // Should be on top of everything else except the target pixmap item (same size as the target pixmap image) which must be topmost to receive events
}

AreaOfInfluenceShapeItem::~AreaOfInfluenceShapeItem()
{
}

}

}
