#pragma once

#include <QByteArray>
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
};

}

}
