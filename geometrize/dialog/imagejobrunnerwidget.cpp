#include "imagejobrunnerwidget.h"
#include "ui_imagejobrunnerwidget.h"

#include <memory>

#include <QHBoxLayout>
#include <QObjectList>
#include <QString>

#include "job/imagejob.h"
#include "layout/flowlayout.h"

namespace geometrize
{

namespace dialog
{

class ImageJobRunnerWidget::ImageJobRunnerWidgetImpl
{
public:
    ImageJobRunnerWidgetImpl(ImageJobRunnerWidget* pQ) : q{pQ}, ui{std::make_unique<Ui::ImageJobRunnerWidget>()}
    {
        ui->setupUi(q);

        auto shapeLayout = new geometrize::layout::FlowLayout();
        shapeLayout->addWidget(ui->usesCircles);
        shapeLayout->addWidget(ui->usesEllipses);
        shapeLayout->addWidget(ui->usesLines);
        shapeLayout->addWidget(ui->usesPolylines);
        shapeLayout->addWidget(ui->usesQuadraticBeziers);
        shapeLayout->addWidget(ui->usesRectangles);
        shapeLayout->addWidget(ui->usesRotatedEllipses);
        shapeLayout->addWidget(ui->usesRotatedRectangles);
        shapeLayout->addWidget(ui->usesTriangles);
        ui->shapeTypesContainer->setLayout(shapeLayout);

        auto runnerButtonsLayout = new geometrize::layout::FlowLayout();
        runnerButtonsLayout->addWidget(ui->runStopButton);
        runnerButtonsLayout->addWidget(ui->stepButton);
        runnerButtonsLayout->addWidget(ui->clearButton);

        ui->bottomButtonsContainer->setLayout(runnerButtonsLayout);
    }
    ~ImageJobRunnerWidgetImpl() = default;
    ImageJobRunnerWidgetImpl operator=(const ImageJobRunnerWidgetImpl&) = delete;
    ImageJobRunnerWidgetImpl(const ImageJobRunnerWidgetImpl&) = delete;

    void setImageJob(job::ImageJob* job)
    {
        m_job = job;
    }

    void setRunStopButtonText(const QString& text)
    {
        ui->runStopButton->setText(text);
    }

    void syncUserInterface()
    {
        geometrize::preferences::ImageJobPreferences& prefs{m_job->getPreferences()};

        const geometrize::ImageRunnerOptions opts{prefs.getImageRunnerOptions()}; // Geometrize library options

        const auto usesShape = [&opts](const geometrize::ShapeTypes type) -> bool {
            const std::uint32_t shapes{static_cast<std::uint32_t>(opts.shapeTypes)};
            return shapes & type;
        };

        ui->usesRectangles->setChecked(usesShape(geometrize::RECTANGLE));
        ui->usesRotatedRectangles->setChecked(usesShape(geometrize::ROTATED_RECTANGLE));
        ui->usesTriangles->setChecked(usesShape(geometrize::TRIANGLE));
        ui->usesEllipses->setChecked(usesShape(geometrize::ELLIPSE));
        ui->usesRotatedEllipses->setChecked(usesShape(geometrize::ROTATED_ELLIPSE));
        ui->usesCircles->setChecked(usesShape(geometrize::CIRCLE));
        ui->usesLines->setChecked(usesShape(geometrize::LINE));
        ui->usesQuadraticBeziers->setChecked(usesShape(geometrize::QUADRATIC_BEZIER));
        ui->usesPolylines->setChecked(usesShape(geometrize::POLYLINE));

        ui->shapeOpacitySlider->setValue(opts.alpha);
        ui->candidateShapesPerStepSlider->setValue(opts.shapeCount);
        ui->mutationsPerCandidateShapeSlider->setValue(opts.maxShapeMutations);
        ui->randomSeedSpinBox->setValue(opts.seed);
        ui->maxThreadsSpinBox->setValue(opts.maxThreads);
    }

    void toggleRunning()
    {
        emit q->runStopButtonClicked();
    }

    void stepModel()
    {
        emit q->stepButtonClicked();
    }

    void clearModel()
    {
        emit q->clearButtonClicked();
    }

    void setShapes(const geometrize::ShapeTypes shapeTypes, const bool enable)
    {
        if(enable) {
            m_job->getPreferences().enableShapeTypes(shapeTypes);
        } else {
            m_job->getPreferences().disableShapeTypes(shapeTypes);
        }
    }

    void setShapeOpacity(const int opacity)
    {
        ui->shapeOpacityValueLabel->setText(QString::number(opacity));

        m_job->getPreferences().setShapeAlpha(opacity);
    }

    void setCandidateShapesPerStep(const int value)
    {
        ui->candidateShapesPerStepCountLabel->setText(QString::number(value));

        m_job->getPreferences().setCandidateShapeCount(value);
    }

    void setMutationsPerCandidateShape(const int value)
    {
        ui->mutationsPerCandidateShapeCountLabel->setText(QString::number(value));

        m_job->getPreferences().setMaxShapeMutations(value);
    }

    void setRandomSeed(const int value)
    {
        m_job->getPreferences().setSeed(value);
    }

    void setMaxThreads(const int value)
    {
        m_job->getPreferences().setMaxThreads(value);
    }

private:
    geometrize::job::ImageJob* m_job;
    ImageJobRunnerWidget* q;
    std::unique_ptr<Ui::ImageJobRunnerWidget> ui;
};

ImageJobRunnerWidget::ImageJobRunnerWidget(QWidget* parent) :
    QWidget{parent},
    d{std::make_unique<ImageJobRunnerWidget::ImageJobRunnerWidgetImpl>(this)}
{
}

ImageJobRunnerWidget::~ImageJobRunnerWidget()
{
}

void ImageJobRunnerWidget::setImageJob(job::ImageJob* job)
{
    d->setImageJob(job);
}

void ImageJobRunnerWidget::setRunStopButtonText(const QString& text)
{
    d->setRunStopButtonText(text);
}

void ImageJobRunnerWidget::syncUserInterface()
{
    d->syncUserInterface();
}

void ImageJobRunnerWidget::on_runStopButton_clicked()
{
    d->toggleRunning();
}

void ImageJobRunnerWidget::on_stepButton_clicked()
{
    d->stepModel();
}

void ImageJobRunnerWidget::on_clearButton_clicked()
{
    d->clearModel();
}

void ImageJobRunnerWidget::on_usesRectangles_clicked(bool checked)
{
    d->setShapes(geometrize::RECTANGLE, checked);
}

void ImageJobRunnerWidget::on_usesRotatedRectangles_clicked(bool checked)
{
    d->setShapes(geometrize::ROTATED_RECTANGLE, checked);
}

void ImageJobRunnerWidget::on_usesTriangles_clicked(bool checked)
{
    d->setShapes(geometrize::TRIANGLE, checked);
}

void ImageJobRunnerWidget::on_usesEllipses_clicked(bool checked)
{
    d->setShapes(geometrize::ELLIPSE, checked);
}

void ImageJobRunnerWidget::on_usesRotatedEllipses_clicked(bool checked)
{
    d->setShapes(geometrize::ROTATED_ELLIPSE, checked);
}

void ImageJobRunnerWidget::on_usesCircles_clicked(bool checked)
{
    d->setShapes(geometrize::CIRCLE, checked);
}

void ImageJobRunnerWidget::on_usesLines_clicked(bool checked)
{
    d->setShapes(geometrize::LINE, checked);
}

void ImageJobRunnerWidget::on_usesQuadraticBeziers_clicked(bool checked)
{
    d->setShapes(geometrize::QUADRATIC_BEZIER, checked);
}

void ImageJobRunnerWidget::on_usesPolylines_clicked(bool checked)
{
    d->setShapes(geometrize::POLYLINE, checked);
}

void ImageJobRunnerWidget::on_shapeOpacitySlider_valueChanged(int value)
{
    d->setShapeOpacity(value);
}

void ImageJobRunnerWidget::on_candidateShapesPerStepSlider_valueChanged(int value)
{
    d->setCandidateShapesPerStep(value);
}

void ImageJobRunnerWidget::on_mutationsPerCandidateShapeSlider_valueChanged(int value)
{
    d->setMutationsPerCandidateShape(value);
}

void ImageJobRunnerWidget::on_randomSeedSpinBox_valueChanged(int value)
{
    d->setRandomSeed(value);
}

void ImageJobRunnerWidget::on_maxThreadsSpinBox_valueChanged(int value)
{
    d->setMaxThreads(value);
}

}

}
