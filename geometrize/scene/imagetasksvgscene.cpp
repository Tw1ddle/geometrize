#include "imagetasksvgscene.h"

#include <memory>

#include <QByteArray>
#include <QGraphicsSvgItem>
#include <QSvgRenderer>

#include "geometrize/shaperesult.h"
#include "geometrize/exporter/svgexporter.h"

#include "scene/imagetaskpixmapgraphicsitem.h"
#include "scene/svgitem.h"

namespace geometrize
{

namespace scene
{

class ImageTaskSvgScene::ImageTaskSvgSceneImpl
{
public:
    ImageTaskSvgSceneImpl(ImageTaskSvgScene* pQ) : q{pQ}
    {
    }
    ImageTaskSvgSceneImpl operator=(const ImageTaskSvgSceneImpl&) = delete;
    ImageTaskSvgSceneImpl(const ImageTaskSvgSceneImpl&) = delete;
    ~ImageTaskSvgSceneImpl() = default;

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
                delete item;
            }
        }
    }

private:
    ImageTaskSvgScene* q;
};

ImageTaskSvgScene::ImageTaskSvgScene() : ImageTaskScene(), d{std::make_unique<ImageTaskSvgScene::ImageTaskSvgSceneImpl>(this)}
{
}

ImageTaskSvgScene::~ImageTaskSvgScene()
{
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
