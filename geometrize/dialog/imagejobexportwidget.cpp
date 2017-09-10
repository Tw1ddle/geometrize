#include "imagejobexportwidget.h"
#include "ui_imagejobexportwidget.h"

#include <memory>
#include <vector>

#include <QMessageBox>

#include "geometrize/shaperesult.h"
#include "geometrize/exporter/shapearrayexporter.h"
#include "geometrize/exporter/shapedataexporter.h"
#include "geometrize/exporter/svgexporter.h"

#include "common/uiactions.h"
#include "common/util.h"
#include "exporter/gifexporter.h"
#include "exporter/imageexporter.h"
#include "exporter/webpageexporter.h"
#include "job/imagejob.h"

namespace geometrize
{

namespace dialog
{

class ImageJobExportWidget::ImageJobExportWidgetImpl
{
public:
    ImageJobExportWidgetImpl(ImageJobExportWidget* pQ) : m_job{nullptr}, m_shapes{nullptr}, q{pQ}, ui{std::make_unique<Ui::ImageJobExportWidget>()}
    {
        ui->setupUi(q);
    }
    ~ImageJobExportWidgetImpl() = default;
    ImageJobExportWidgetImpl operator=(const ImageJobExportWidgetImpl&) = delete;
    ImageJobExportWidgetImpl(const ImageJobExportWidgetImpl&) = delete;

    void setImageJob(const job::ImageJob* job, const std::vector<geometrize::ShapeResult>* shapes)
    {
        m_job = job;
        m_shapes = shapes;
    }

    void saveSVG() const
    {
        if(!m_job || !m_shapes) {
            showExportMisconfiguredMessage();
            return;
        }

        const QString path{common::ui::openSaveSVGPathPickerDialog(q)};
        if(path.isEmpty()) {
            return;
        }

        const std::string data{geometrize::exporter::exportSVG(*m_shapes, m_job->getCurrent().getWidth(), m_job->getCurrent().getHeight())};
        util::writeStringToFile(data, path.toStdString());
    }

    void saveRasterizedSVG() const
    {
        if(!m_job || !m_shapes) {
            showExportMisconfiguredMessage();
            return;
        }

        const QString path{common::ui::openSaveRasterizedSVGPathPickerDialog(q)};
        if(path.isEmpty()) {
            return;
        }

        // Rasterize as x3 normal size and downscale for less jaggy result
        const std::uint32_t scaleFactor{3};
        const std::uint32_t width{m_job->getCurrent().getWidth()};
        const std::uint32_t height{m_job->getCurrent().getHeight()};
        geometrize::exporter::exportRasterizedSvg(
                    *m_shapes,
                    width,
                    height,
                    width * scaleFactor,
                    height * scaleFactor,
                    path.toStdString());
    }

    void saveRasterizedSVGs() const
    {
        if(!m_job || !m_shapes) {
            showExportMisconfiguredMessage();
            return;
        }

        const QString path{common::ui::openSaveRasterizedSVGsPathPickerDialog(q)};
        if(path.isEmpty()) {
            return;
        }

        // Rasterize as x3 normal size and downscale for less jaggy result
        const std::uint32_t scaleFactor{3};
        const std::uint32_t width{m_job->getCurrent().getWidth()};
        const std::uint32_t height{m_job->getCurrent().getHeight()};
        geometrize::exporter::exportRasterizedSvgs(
                    *m_shapes,
                    width,
                    height,
                    width * scaleFactor,
                    height * scaleFactor,
                    path.toStdString(),
                    "exported_image",
                    ".png");
    }

    void saveGeometryData() const
    {
        if(!m_shapes) {
            showExportMisconfiguredMessage();
            return;
        }

        const QString path{common::ui::openSaveGeometryDataPathPickerDialog(q)};
        if(path.isEmpty()) {
            return;
        }

        geometrize::exporter::ShapeDataFormat format = exporter::ShapeDataFormat::JSON;
        if(path.endsWith("json")) {
            format = geometrize::exporter::ShapeDataFormat::JSON;
        } else if(path.endsWith("txt")) {
            format = geometrize::exporter::ShapeDataFormat::CUSTOM_ARRAY;
        }

        const std::string data{geometrize::exporter::exportShapeData(*m_shapes, format)};
        util::writeStringToFile(data, path.toStdString());
    }

    void saveGIF() const
    {
        if(!m_job || !m_shapes) {
            showExportMisconfiguredMessage();
            return;
        }

        const QString path{common::ui::openSaveGIFPathPickerDialog(q)};
        if(path.isEmpty()) {
            return;
        }

        // Rasterize as x3 normal size and downscale for less jaggy result
        const std::uint32_t scaleFactor{3};
        const std::uint32_t width{m_job->getCurrent().getWidth()};
        const std::uint32_t height{m_job->getCurrent().getHeight()};
        geometrize::exporter::exportGIF(
            *m_shapes,
            width,
            height,
            width * scaleFactor,
            height * scaleFactor,
            path.toStdString());
    }

    void saveHTML5WebpageButton() const
    {
        if(!m_shapes) {
            showExportMisconfiguredMessage();
            return;
        }

        const QString path{common::ui::openSaveCanvasPathPickerDialog(q)};
        if(path.isEmpty()) {
            return;
        }

        const std::string pageSource{geometrize::exporter::exportCanvasWebpage(*m_shapes)};
        util::writeStringToFile(pageSource, path.toStdString());
    }

    void saveWebGLWebpageButton() const
    {
        if(!m_shapes) {
            showExportMisconfiguredMessage();
            return;
        }

        const QString path{common::ui::openSaveWebGLPathPickerDialog(q)};
        if(path.isEmpty()) {
            return;
        }

        const std::string pageSource{geometrize::exporter::exportWebGLWebpage(*m_shapes)};
        util::writeStringToFile(pageSource, path.toStdString());
    }

private:
    void showExportMisconfiguredMessage() const
    {
        QMessageBox::warning(q, q->tr("Export failed"), q->tr("Failed to run exporter. Exporter was misconfigured."));
    }

    const geometrize::job::ImageJob* m_job;
    const std::vector<geometrize::ShapeResult>* m_shapes;

    ImageJobExportWidget* q;
    std::unique_ptr<Ui::ImageJobExportWidget> ui;
};

ImageJobExportWidget::ImageJobExportWidget(QWidget* parent) :
    QWidget{parent},
    d{std::make_unique<ImageJobExportWidget::ImageJobExportWidgetImpl>(this)}
{
}

ImageJobExportWidget::~ImageJobExportWidget()
{
}

void ImageJobExportWidget::setImageJob(const job::ImageJob* job, const std::vector<geometrize::ShapeResult>* shapes)
{
    d->setImageJob(job, shapes);
}

void ImageJobExportWidget::on_saveImageButton_clicked()
{
    d->saveRasterizedSVG();
}

void ImageJobExportWidget::on_saveImagesButton_clicked()
{
    d->saveRasterizedSVGs();
}

void ImageJobExportWidget::on_saveSVGButton_clicked()
{
    d->saveSVG();
}

void ImageJobExportWidget::on_saveGeometryDataButton_clicked()
{
    d->saveGeometryData();
}

void ImageJobExportWidget::on_saveGIFButton_clicked()
{
    d->saveGIF();
}

void ImageJobExportWidget::on_saveHTML5WebpageButton_clicked()
{
    d->saveHTML5WebpageButton();
}

void ImageJobExportWidget::on_saveWebGLWebpageButton_clicked()
{
    d->saveWebGLWebpageButton();
}

}

}
