#include "imagejobsvgscene.h"

#include <memory>

#include <QByteArray>
#include <QGraphicsSvgItem>
#include <QSvgRenderer>

#include "geometrize/exporter/svgexporter.h"

#include "dialog/imagejobpixmapgraphicsitem.h"

namespace geometrize
{

namespace dialog
{

class SvgItem : public QGraphicsSvgItem
{
public:
    SvgItem(const QByteArray& data) : QGraphicsSvgItem{}
    {
        setSharedRenderer(new QSvgRenderer(data));
    }
    ~SvgItem()
    {
        delete renderer();
    }
};

class ImageJobSvgScene::ImageJobSvgSceneImpl
{
public:
    ImageJobSvgSceneImpl(ImageJobSvgScene* pQ) : q{pQ}, m_targetPixmapItem{new ImageJobPixmapGraphicsItem()}
    {
        m_targetPixmapItem->setZValue(1);
        q->addItem(m_targetPixmapItem);
    }
    ImageJobSvgSceneImpl operator=(const ImageJobSvgSceneImpl&) = delete;
    ImageJobSvgSceneImpl(const ImageJobSvgSceneImpl&) = delete;
    ~ImageJobSvgSceneImpl() = default;

    void setTargetPixmap(const QPixmap& pixmap)
    {
        m_targetPixmapItem->setPixmap(pixmap);
    }

    void setTargetPixmapOpacity(const float opacity)
    {
        m_targetPixmapItem->setOpacity(opacity);
    }

    void drawSvg(const std::vector<geometrize::ShapeResult>& shapes, const std::uint32_t width, const std::uint32_t height)
    {
        if(shapes.empty()) {
            return;
        }

        const QByteArray svgData{QByteArray::fromStdString(geometrize::exporter::exportSVG(shapes, width, height))};
        auto svgItem = new SvgItem(svgData);
        svgItem->setFlags(QGraphicsItem::ItemClipsToShape);
        svgItem->setCacheMode(QGraphicsItem::NoCache);
        q->addItem(svgItem);
        svgItem->setZValue(0);
    }

private:
    ImageJobSvgScene* q;
    ImageJobPixmapGraphicsItem* m_targetPixmapItem;
};

ImageJobSvgScene::ImageJobSvgScene() : QGraphicsScene(), d{std::make_unique<ImageJobSvgScene::ImageJobSvgSceneImpl>(this)}
{
}

ImageJobSvgScene::~ImageJobSvgScene()
{
}

void ImageJobSvgScene::setTargetPixmap(const QPixmap& pixmap)
{
    d->setTargetPixmap(pixmap);
}

void ImageJobSvgScene::setTargetPixmapOpacity(const float opacity)
{
    d->setTargetPixmapOpacity(opacity);
}

void ImageJobSvgScene::drawSvg(const std::vector<geometrize::ShapeResult>& shapes, const std::uint32_t width, const std::uint32_t height)
{
    d->drawSvg(shapes, width, height);
}

}

}
