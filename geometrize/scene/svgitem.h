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

    // NOTE unfortunately there isn't much point overriding the pointer events, because transparent parts of the image aren't ignored
    // https://stackoverflow.com/questions/5026429/ignore-mouse-events-over-transparent-parts-of-an-svg-image-in-qgraphicsview
};

}

}
