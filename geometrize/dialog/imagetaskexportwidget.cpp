#include "imagetaskexportwidget.h"
#include "ui_imagetaskexportwidget.h"

#include <memory>
#include <vector>

#include <QMessageBox>

#include "geometrize/shaperesult.h"
#include "geometrize/exporter/shapearrayexporter.h"
#include "geometrize/exporter/svgexporter.h"

#include "common/uiactions.h"
#include "common/util.h"
#include "exporter/gifexporter.h"
#include "exporter/imageexporter.h"
#include "exporter/shapedataexporter.h"
#include "exporter/webpageexporter.h"
#include "task/imagetask.h"

namespace geometrize
{

namespace dialog
{

class ImageTaskExportWidget::ImageTaskExportWidgetImpl
{
public:
    ImageTaskExportWidgetImpl(ImageTaskExportWidget* pQ) : m_task{nullptr}, m_shapes{nullptr}, q{pQ}, ui{std::make_unique<Ui::ImageTaskExportWidget>()}
    {
        ui->setupUi(q);
        populateUi();
    }
    ~ImageTaskExportWidgetImpl() = default;
    ImageTaskExportWidgetImpl operator=(const ImageTaskExportWidgetImpl&) = delete;
    ImageTaskExportWidgetImpl(const ImageTaskExportWidgetImpl&) = delete;

    void setImageTask(const task::ImageTask* task, const std::vector<geometrize::ShapeResult>* shapes)
    {
        m_task = task;
        m_shapes = shapes;
    }

    void saveSVG() const
    {
        if(!m_task || !m_shapes) {
            showExportMisconfiguredMessage();
            return;
        }

        const QString path{common::ui::openSaveSVGPathPickerDialog(q)};
        if(path.isEmpty()) {
            return;
        }

        const std::string data{geometrize::exporter::exportSVG(*m_shapes, m_task->getCurrent().getWidth(), m_task->getCurrent().getHeight())};
        util::writeStringToFile(data, path.toStdString());
    }

    void saveRasterizedSVG() const
    {
        if(!m_task || !m_shapes) {
            showExportMisconfiguredMessage();
            return;
        }

        const QString path{common::ui::openSaveRasterizedSVGPathPickerDialog(q)};
        if(path.isEmpty()) {
            return;
        }

        // Rasterize as x3 normal size and downscale for less jaggy result
        const std::uint32_t scaleFactor{3};
        const std::uint32_t width{m_task->getCurrent().getWidth()};
        const std::uint32_t height{m_task->getCurrent().getHeight()};
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
        if(!m_task || !m_shapes) {
            showExportMisconfiguredMessage();
            return;
        }

        const QString path{common::ui::openSaveRasterizedSVGsPathPickerDialog(q)};
        if(path.isEmpty()) {
            return;
        }

        // Rasterize as x3 normal size and downscale for less jaggy result
        const std::uint32_t scaleFactor{3};
        const std::uint32_t width{m_task->getCurrent().getWidth()};
        const std::uint32_t height{m_task->getCurrent().getHeight()};
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
        if(!m_task || !m_shapes) {
            showExportMisconfiguredMessage();
            return;
        }

        const QString path{common::ui::openSaveGIFPathPickerDialog(q)};
        if(path.isEmpty()) {
            return;
        }

        // Rasterize as x3 normal size and downscale for less jaggy result
        const std::uint32_t scaleFactor{3};
        const std::uint32_t width{m_task->getCurrent().getWidth()};
        const std::uint32_t height{m_task->getCurrent().getHeight()};

        auto frameSkipPredicate = [](const std::size_t frameIdx) {
            if(frameIdx % 2 == 0) {
                return true;
            }
            return false;
        };

        geometrize::exporter::exportGIF(
            *m_shapes,
            width,
            height,
            width * scaleFactor,
            height * scaleFactor,
            frameSkipPredicate,
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

    void onLanguageChange()
    {
        ui->retranslateUi(q);
        populateUi();
    }

private:
    void populateUi()
    {
    }

    void showExportMisconfiguredMessage() const
    {
        QMessageBox::warning(q, tr("Failed to run exporter", "Title of error message shown when an attempt to save/export a file failed"),
                             tr("Failed to run exporter. Exporter was misconfigured.", "Error message text shown when an attempt to save/export a file failed"));
    }

    const geometrize::task::ImageTask* m_task;
    const std::vector<geometrize::ShapeResult>* m_shapes;

    ImageTaskExportWidget* q;
    std::unique_ptr<Ui::ImageTaskExportWidget> ui;
};

ImageTaskExportWidget::ImageTaskExportWidget(QWidget* parent) :
    QWidget{parent},
    d{std::make_unique<ImageTaskExportWidget::ImageTaskExportWidgetImpl>(this)}
{
}

ImageTaskExportWidget::~ImageTaskExportWidget()
{
}

void ImageTaskExportWidget::setImageTask(const task::ImageTask* task, const std::vector<geometrize::ShapeResult>* shapes)
{
    d->setImageTask(task, shapes);
}

void ImageTaskExportWidget::on_saveImageButton_clicked()
{
    d->saveRasterizedSVG();
}

void ImageTaskExportWidget::on_saveImagesButton_clicked()
{
    d->saveRasterizedSVGs();
}

void ImageTaskExportWidget::on_saveSVGButton_clicked()
{
    d->saveSVG();
}

void ImageTaskExportWidget::on_saveGeometryDataButton_clicked()
{
    d->saveGeometryData();
}

void ImageTaskExportWidget::on_saveGIFButton_clicked()
{
    d->saveGIF();
}

void ImageTaskExportWidget::on_saveHTML5WebpageButton_clicked()
{
    d->saveHTML5WebpageButton();
}

void ImageTaskExportWidget::on_saveWebGLWebpageButton_clicked()
{
    d->saveWebGLWebpageButton();
}

void ImageTaskExportWidget::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        d->onLanguageChange();
    }
    QWidget::changeEvent(event);
}

}

}
