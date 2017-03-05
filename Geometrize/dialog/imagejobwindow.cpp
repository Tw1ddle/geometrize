#include "imagejobwindow.h"
#include "ui_imagejobwindow.h"

#include <assert.h>
#include <fstream>

#include <QCloseEvent>
#include <QDesktopServices>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QPixmap>

#include "geometrize/bitmap/bitmap.h"
#include "geometrize/core.h"
#include "geometrize/exporter/bitmapdataexporter.h"
#include "geometrize/exporter/bitmapexporter.h"
#include "geometrize/runner/imagerunneroptions.h"
#include "geometrize/exporter/svgexporter.h"

#include "common/uiactions.h"
#include "constants.h"
#include "dialog/aboutdialog.h"
#include "dialog/collapsiblesection.h"
#include "dialog/globalpreferencestabdialog.h"
#include "exporter/gifexporter.h"
#include "exporter/imageexporter.h"
#include "exporter/canvasanimationexporter.h"
#include "exporter/shapedataexporter.h"
#include "exporter/webglanimationexporter.h"
#include "image/imageloader.h"
#include "job/imagejob.h"
#include "util.h"

namespace geometrize
{

namespace dialog
{

class ImageJobWindow::ImageJobWindowImpl
{
public:
    ImageJobWindowImpl(ImageJobWindow* pQ) : ui{std::make_unique<Ui::ImageJobWindow>()}, q{pQ}, m_job{nullptr}, m_running{false}, m_initialJobImage{nullptr}
    {
        ui->setupUi(q);

        ui->imageView->setScene(&m_scene);
        m_scene.addItem(&m_currentPixmapItem);
        m_scene.addItem(&m_targetPixmapItem);

        connect(ui->targetOpacitySlider, &QSlider::valueChanged, [this](int value) {
            m_targetPixmapItem.setOpacity(value * 0.01f);
        });
    }
    ImageJobWindowImpl operator=(const ImageJobWindowImpl&) = delete;
    ImageJobWindowImpl(const ImageJobWindowImpl&) = delete;
    ~ImageJobWindowImpl() = default;

    void close()
    {
        q->close();
    }

    void setImageJob(job::ImageJob* job)
    {
        assert(!m_job && "Changing image job once one has already been set is currently unsupported");
        m_job = job;

        m_initialJobImage = std::make_unique<geometrize::Bitmap>(m_job->getCurrent());

        setupOverlayImages();
        updateWorkingImage();

        setDisplayName(QString::fromStdString(m_job->getDisplayName()));

        syncUserInterfaceWithOptions();

        connect(job, &job::ImageJob::signal_modelDidStep, [this](std::vector<geometrize::ShapeResult> shapes) {
            updateWorkingImage();

            if(m_running) {
                stepModel();
            }

            std::copy(shapes.begin(), shapes.end(), std::back_inserter(m_shapes));
            ui->shapeCountValueLabel->setText(QString::number(m_shapes.size()));
        });
    }

    void toggleRunning()
    {
        m_running = !m_running;

        if(m_running) {
            stepModel();
            ui->runStopButton->setText(tr("Stop"));
        } else {
            ui->runStopButton->setText(tr("Run"));
        }
    }

    void stepModel()
    {
        m_job->stepModel();
    }

    void revealLaunchWindow()
    {
        if(common::ui::isLaunchWindowOpen()) {
            common::ui::bringLaunchWindowToFront();
        } else {
            common::ui::openLaunchWindow();
        }
    }

    void loadSettingsTemplate()
    {
        const QString path{common::ui::openLoadImageJobSettingsDialog(q)};
        if(path.isEmpty()) {
            return;
        }

        m_job->getPreferences().load(path.toStdString());
    }

    void saveSettingsTemplate() const
    {
        const QString path{common::ui::openSaveImageJobSettingsDialog(q)};
        if(path.isEmpty()) {
            return;
        }

        m_job->getPreferences().save(path.toStdString());
    }

    void saveImage() const
    {
        const QString path{common::ui::openSaveImagePathPickerDialog(q)};
        if(path.isEmpty()) {
            return;
        }

        exporter::exportImage(m_job->getCurrent(), path.toStdString());
    }

    void saveSVG() const
    {
        const QString path{common::ui::openSaveSVGPathPickerDialog(q)};
        if(path.isEmpty()) {
            return;
        }

        // TODO record added shapes and pass them to export svg data
        //exporter::exportSvg()
    }

    void saveGeometryData() const
    {
        const QString path{common::ui::openSaveGeometryDataPathPickerDialog(q)};
        if(path.isEmpty()) {
            return;
        }

        exporter::ShapeDataFormat format = exporter::ShapeDataFormat::JSON;
        if(path.endsWith("json")) {
            format = exporter::ShapeDataFormat::JSON;
        } else if(path.endsWith("txt")) {
            format = exporter::ShapeDataFormat::CUSTOM_ARRAY;
        }

        const std::string data{exporter::exportShapeData(m_shapes, format)};
        util::writeStringToFile(data, path.toStdString());
    }

    void saveGIF() const
    {
        const QString path{common::ui::openSaveGIFPathPickerDialog(q)};
        if(path.isEmpty()) {
            return;
        }

        exporter::exportGIF(*m_initialJobImage, m_job->getTarget(), m_shapes, path.toStdString());
    }

    void saveCanvasAnimation() const
    {
        const QString path{common::ui::openSaveCanvasAnimationPathPickerDialog(q)};
        if(path.isEmpty()) {
            return;
        }

        const std::string pageSource{exporter::exportCanvasAnimation(m_shapes)};
        util::writeStringToFile(pageSource, path.toStdString());
    }

    void saveWebGLAnimation() const
    {
        const QString path{common::ui::openSaveWebGLPathPickerDialog(q)};
        if(path.isEmpty()) {
            return;
        }

        const std::string pageSource{exporter::exportWebGLAnimation(m_shapes)};
        util::writeStringToFile(pageSource, path.toStdString());
    }

    void saveRawImageDataButton() const
    {
        const QString path{common::ui::openSaveRawImageDataPathPickerDialog(q)};
        if(path.isEmpty()) {
            return;
        }

        exporter::exportBitmapData(m_job->getCurrent(), path.toStdString());
    }

    void setShapes(const geometrize::shapes::ShapeTypes shapeTypes, const bool enable)
    {
        if(enable) {
            m_job->getPreferences().enableShapeTypes(shapeTypes);
        } else {
            m_job->getPreferences().disableShapeTypes(shapeTypes);
        }
    }

    void setShapeOpacity(const int opacity)
    {
        m_job->getPreferences().setShapeAlpha(opacity);
    }

    void setCandidateShapesPerStep(const int value)
    {
        m_job->getPreferences().setCandidateShapeCount(value);
    }

    void setMutationsPerCandidateShape(const int value)
    {
        m_job->getPreferences().setMaxShapeMutations(value);
    }

    void setPasses(const int value)
    {
        m_job->getPreferences().setPasses(value);
    }

    void setMaxThreads(const int value)
    {
        // TODO
    }

private:
    void setDisplayName(const QString& displayName)
    {
        q->setWindowTitle(tr("Geometrize - Image - %1").arg(displayName));
    }

    void updateWorkingImage()
    {
        const QPixmap pixmap{image::createPixmap(m_job->getCurrent())};
        m_currentPixmapItem.setPixmap(pixmap);
    }

    void setupOverlayImages()
    {
        const QPixmap target{image::createPixmap(m_job->getTarget())};
        m_targetPixmapItem.setPixmap(target);
    }

    void syncUserInterfaceWithOptions()
    {
        const geometrize::ImageRunnerOptions opts{m_job->getPreferences().getImageRunnerOptions()}; // Geometrize library options

        const float startingTargetOpacity{10};
        ui->targetOpacitySlider->setValue(startingTargetOpacity);

        const auto usesShape = [&opts](const geometrize::shapes::ShapeTypes type) -> bool {
            const std::uint32_t shapes{static_cast<std::uint32_t>(opts.shapeTypes)};
            return shapes & type;
        };

        ui->usesRectangles->setChecked(usesShape(shapes::RECTANGLE));
        ui->usesRotatedRectangles->setChecked(usesShape(shapes::ROTATED_RECTANGLE));
        ui->usesTriangles->setChecked(usesShape(shapes::TRIANGLE));
        ui->usesEllipses->setChecked(usesShape(shapes::ELLIPSE));
        ui->usesRotatedEllipses->setChecked(usesShape(shapes::ROTATED_ELLIPSE));
        ui->usesCircles->setChecked(usesShape(shapes::CIRCLE));
        ui->usesLines->setChecked(usesShape(shapes::LINE));
        ui->usesSplines->setChecked(usesShape(shapes::SPLINE));

        ui->shapeOpacitySlider->setValue(opts.alpha);
        ui->candidateShapesPerStepSlider->setValue(opts.shapeCount);
        ui->mutationsPerCandidateShapeSlider->setValue(opts.maxShapeMutations);
        ui->passesSlider->setValue(opts.passes);
        //ui->maxThreadsSlider->setValue(opts.maxThreads); // TODO
    }

    job::ImageJob* m_job;
    ImageJobWindow* q;
    QGraphicsScene m_scene;
    std::unique_ptr<Ui::ImageJobWindow> ui;

    std::unique_ptr<geometrize::Bitmap> m_initialJobImage;
    std::vector<geometrize::ShapeResult> m_shapes;

    QGraphicsPixmapItem m_targetPixmapItem;
    QGraphicsPixmapItem m_currentPixmapItem;

    bool m_running; // Whether the model is running (automatically)
};

ImageJobWindow::ImageJobWindow() :
    QMainWindow(nullptr),
    d{std::make_unique<ImageJobWindow::ImageJobWindowImpl>(this)}
{
}

ImageJobWindow::~ImageJobWindow()
{

}

void ImageJobWindow::setImageJob(job::ImageJob* job)
{
    d->setImageJob(job);
}

void ImageJobWindow::on_actionExit_triggered()
{
    d->close();
}

void ImageJobWindow::on_actionLoad_Settings_Template_triggered()
{
    d->loadSettingsTemplate();
}

void ImageJobWindow::on_actionSave_Settings_Template_triggered()
{
    d->saveSettingsTemplate();
}

void ImageJobWindow::on_actionReveal_Launch_Window_triggered()
{
    d->revealLaunchWindow();
}

void ImageJobWindow::on_runStopButton_clicked()
{
    d->toggleRunning();
}

void ImageJobWindow::on_stepButton_clicked()
{
    d->stepModel();
}

void ImageJobWindow::on_saveImageButton_clicked()
{
    d->saveImage();
}

void ImageJobWindow::on_saveSVGButton_clicked()
{
    d->saveSVG();
}

void ImageJobWindow::on_saveGeometryDataButton_clicked()
{
    d->saveGeometryData();
}

void ImageJobWindow::on_saveGIFButton_clicked()
{
    d->saveGIF();
}

void ImageJobWindow::on_saveCanvasAnimationButton_clicked()
{
    d->saveCanvasAnimation();
}

void ImageJobWindow::on_saveWebGLButton_clicked()
{
    d->saveWebGLAnimation();
}

void ImageJobWindow::on_saveRawImageDataButton_clicked()
{
    d->saveRawImageDataButton();
}

void ImageJobWindow::on_usesRectangles_clicked(bool checked)
{
    d->setShapes(geometrize::shapes::RECTANGLE, checked);
}

void ImageJobWindow::on_usesRotatedRectangles_clicked(bool checked)
{
    d->setShapes(geometrize::shapes::ROTATED_RECTANGLE, checked);
}

void ImageJobWindow::on_usesTriangles_clicked(bool checked)
{
    d->setShapes(geometrize::shapes::TRIANGLE, checked);
}

void ImageJobWindow::on_usesEllipses_clicked(bool checked)
{
    d->setShapes(geometrize::shapes::ELLIPSE, checked);
}

void ImageJobWindow::on_usesRotatedEllipses_clicked(bool checked)
{
    d->setShapes(geometrize::shapes::ROTATED_ELLIPSE, checked);
}

void ImageJobWindow::on_usesCircles_clicked(bool checked)
{
    d->setShapes(geometrize::shapes::CIRCLE, checked);
}

void ImageJobWindow::on_usesLines_clicked(bool checked)
{
    d->setShapes(geometrize::shapes::LINE, checked);
}

void ImageJobWindow::on_usesSplines_clicked(bool checked)
{
    d->setShapes(geometrize::shapes::SPLINE, checked);
}

void ImageJobWindow::on_shapeOpacitySlider_valueChanged(int value)
{
    d->setShapeOpacity(value);
}

void ImageJobWindow::on_candidateShapesPerStepSlider_valueChanged(int value)
{
    d->setCandidateShapesPerStep(value);
}

void ImageJobWindow::on_mutationsPerCandidateShapeSlider_valueChanged(int value)
{
    d->setMutationsPerCandidateShape(value);
}

void ImageJobWindow::on_passesSlider_valueChanged(int value)
{
    d->setPasses(value);
}

void ImageJobWindow::on_maxThreadsSlider_valueChanged(int value)
{
    d->setMaxThreads(value);
}

}

}
