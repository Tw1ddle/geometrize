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
        q->removeItem(m_targetPixmapItem);
        q->clear();

        q->addItem(m_targetPixmapItem);

        if(shapes.empty()) {
            return;
        }

        // NOTE passing 0, 0 in here happens to work - should really use background size (probably first shape) or bounding box of all shapes
        const QByteArray svgData{QByteArray::fromStdString(geometrize::exporter::exportSVG(shapes, width, height))};

        m_svgItem = new SvgItem(svgData);
        m_svgItem->setFlags(QGraphicsItem::ItemClipsToShape);
        m_svgItem->setCacheMode(QGraphicsItem::NoCache);

        q->addItem(m_svgItem);
    }

private:
    ImageJobSvgScene* q;
    ImageJobPixmapGraphicsItem* m_targetPixmapItem;
    SvgItem* m_svgItem;
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
