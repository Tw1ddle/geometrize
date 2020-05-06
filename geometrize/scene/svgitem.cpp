#include "svgitem.h"

#include <QByteArray>
#include <QSvgRenderer>

namespace geometrize
{

namespace scene
{

SvgItem::SvgItem(const QByteArray& data) : QGraphicsSvgItem{}
{
    setSharedRenderer(new QSvgRenderer(data));
    setFlag(ItemIsMovable, false);
}

SvgItem::~SvgItem()
{
	delete renderer();
}

}

}
