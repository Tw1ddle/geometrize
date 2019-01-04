#include "imagetasksvgscene.h"

#include <memory>
#include <vector>

#include <QByteArray>
#include <QGraphicsSvgItem>
#include <QSvgRenderer>

#include "geometrize/shaperesult.h"
#include "geometrize/exporter/svgexporter.h"

#include "dialog/imagetaskpixmapgraphicsitem.h"

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

class ImageTaskSvgScene::ImageTaskSvgSceneImpl
{
public:
    ImageTaskSvgSceneImpl(ImageTaskSvgScene* pQ) : q{pQ}, m_targetPixmapItem{new ImageTaskPixmapGraphicsItem()}
    {
        m_targetPixmapItem->setZValue(1);
        q->addItem(m_targetPixmapItem);
    }
    ImageTaskSvgSceneImpl operator=(const ImageTaskSvgSceneImpl&) = delete;
    ImageTaskSvgSceneImpl(const ImageTaskSvgSceneImpl&) = delete;
    ~ImageTaskSvgSceneImpl() = default;

    void setTargetPixmap(const QPixmap& pixmap)
    {
        m_targetPixmapItem->setPixmap(pixmap);
    }

    void setTargetPixmapOpacity(const float opacity)
    {
        m_targetPixmapItem->setOpacity(opacity);
    }

    void addShapes(const std::vector<geometrize::ShapeResult>& shapes, const std::uint32_t width, const std::uint32_t height)
    {
        if(shapes.empty()) {
            return;
        }

        const QString svgString{QString::fromStdString(geometrize::exporter::exportSVG(shapes, width, height))};
        const QByteArray svgData{svgString.toUtf8()};

        auto svgItem = new SvgItem(svgData);
        svgItem->setFlags(QGraphicsItem::ItemClipsToShape);
        svgItem->setCacheMode(QGraphicsItem::NoCache);
        q->addItem(svgItem);
        svgItem->setZValue(0);
    }

    void removeShapes()
    {
        const auto items = q->items();
        for(auto& item : items) {
            if(dynamic_cast<SvgItem*>(item) != nullptr) {
                q->removeItem(item);
            }
        }
    }

private:
    ImageTaskSvgScene* q;
    ImageTaskPixmapGraphicsItem* m_targetPixmapItem;
};

ImageTaskSvgScene::ImageTaskSvgScene() : QGraphicsScene(), d{std::make_unique<ImageTaskSvgScene::ImageTaskSvgSceneImpl>(this)}
{
}

ImageTaskSvgScene::~ImageTaskSvgScene()
{
}

void ImageTaskSvgScene::setTargetPixmap(const QPixmap& pixmap)
{
    d->setTargetPixmap(pixmap);
}

void ImageTaskSvgScene::setTargetPixmapOpacity(const float opacity)
{
    d->setTargetPixmapOpacity(opacity);
}

void ImageTaskSvgScene::addShapes(const std::vector<geometrize::ShapeResult>& shapes, const std::uint32_t width, const std::uint32_t height)
{
    d->addShapes(shapes, width, height);
}

void ImageTaskSvgScene::removeShapes()
{
    d->removeShapes();
}

}

}
