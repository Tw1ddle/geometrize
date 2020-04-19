#include "imagetaskrunnerwidget.h"
#include "ui_imagetaskrunnerwidget.h"

#include <memory>

#include <QEvent>
#include <QLocale>
#include <QString>

#include "layout/flowlayout.h"
#include "preferences/globalpreferences.h"
#include "task/imagetask.h"

namespace geometrize
{

namespace dialog
{

class ImageTaskRunnerWidget::ImageTaskRunnerWidgetImpl
{
public:
    ImageTaskRunnerWidgetImpl(ImageTaskRunnerWidget* pQ) : q{pQ}, ui{std::make_unique<Ui::ImageTaskRunnerWidget>()}
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

        populateUi();
    }
    ~ImageTaskRunnerWidgetImpl() = default;
    ImageTaskRunnerWidgetImpl operator=(const ImageTaskRunnerWidgetImpl&) = delete;
    ImageTaskRunnerWidgetImpl(const ImageTaskRunnerWidgetImpl&) = delete;

    void setImageTask(task::ImageTask* task)
    {
        m_task = task;
    }

    void setRunStopButtonText(const QString& text)
    {
        ui->runStopButton->setText(text);
    }

    void syncUserInterface()
    {
        if(m_task == nullptr) {
            return;
        }

        geometrize::preferences::ImageTaskPreferences& prefs{m_task->getPreferences()};

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

        // Constrain max threads based on global settings
        ui->maxThreadsSpinBox->setMaximum(geometrize::preferences::getGlobalPreferences().getImageTaskMaxThreads());

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
            m_task->getPreferences().enableShapeTypes(shapeTypes);
        } else {
            m_task->getPreferences().disableShapeTypes(shapeTypes);
        }
    }

    void setShapeOpacity(const int opacity)
    {
        ui->shapeOpacityValueLabel->setText(QLocale().toString(opacity));

        m_task->getPreferences().setShapeAlpha(opacity);
    }

    void setCandidateShapesPerStep(const int value)
    {
        ui->candidateShapesPerStepCountLabel->setText(QLocale().toString(value));

        m_task->getPreferences().setCandidateShapeCount(value);
    }

    void setMutationsPerCandidateShape(const int value)
    {
        ui->mutationsPerCandidateShapeCountLabel->setText(QLocale().toString(value));

        m_task->getPreferences().setMaxShapeMutations(value);
    }

    void setRandomSeed(const int value)
    {
        m_task->getPreferences().setSeed(value);
    }

    void setMaxThreads(const int value)
    {
        m_task->getPreferences().setMaxThreads(value);
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

    geometrize::task::ImageTask* m_task;
    ImageTaskRunnerWidget* q;
    std::unique_ptr<Ui::ImageTaskRunnerWidget> ui;
};

ImageTaskRunnerWidget::ImageTaskRunnerWidget(QWidget* parent) :
    QWidget{parent},
    d{std::make_unique<ImageTaskRunnerWidget::ImageTaskRunnerWidgetImpl>(this)}
{
}

ImageTaskRunnerWidget::~ImageTaskRunnerWidget()
{
}

void ImageTaskRunnerWidget::setImageTask(task::ImageTask* task)
{
    d->setImageTask(task);
}

void ImageTaskRunnerWidget::setRunStopButtonText(const QString& text)
{
    d->setRunStopButtonText(text);
}

void ImageTaskRunnerWidget::syncUserInterface()
{
    d->syncUserInterface();
}

void ImageTaskRunnerWidget::on_runStopButton_clicked()
{
    d->toggleRunning();
}

void ImageTaskRunnerWidget::on_stepButton_clicked()
{
    d->stepModel();
}

void ImageTaskRunnerWidget::on_clearButton_clicked()
{
    d->clearModel();
}

void ImageTaskRunnerWidget::on_usesRectangles_clicked(bool checked)
{
    d->setShapes(geometrize::RECTANGLE, checked);
}

void ImageTaskRunnerWidget::on_usesRotatedRectangles_clicked(bool checked)
{
    d->setShapes(geometrize::ROTATED_RECTANGLE, checked);
}

void ImageTaskRunnerWidget::on_usesTriangles_clicked(bool checked)
{
    d->setShapes(geometrize::TRIANGLE, checked);
}

void ImageTaskRunnerWidget::on_usesEllipses_clicked(bool checked)
{
    d->setShapes(geometrize::ELLIPSE, checked);
}

void ImageTaskRunnerWidget::on_usesRotatedEllipses_clicked(bool checked)
{
    d->setShapes(geometrize::ROTATED_ELLIPSE, checked);
}

void ImageTaskRunnerWidget::on_usesCircles_clicked(bool checked)
{
    d->setShapes(geometrize::CIRCLE, checked);
}

void ImageTaskRunnerWidget::on_usesLines_clicked(bool checked)
{
    d->setShapes(geometrize::LINE, checked);
}

void ImageTaskRunnerWidget::on_usesQuadraticBeziers_clicked(bool checked)
{
    d->setShapes(geometrize::QUADRATIC_BEZIER, checked);
}

void ImageTaskRunnerWidget::on_usesPolylines_clicked(bool checked)
{
    d->setShapes(geometrize::POLYLINE, checked);
}

void ImageTaskRunnerWidget::on_shapeOpacitySlider_valueChanged(int value)
{
    d->setShapeOpacity(value);
}

void ImageTaskRunnerWidget::on_candidateShapesPerStepSlider_valueChanged(int value)
{
    d->setCandidateShapesPerStep(value);
}

void ImageTaskRunnerWidget::on_mutationsPerCandidateShapeSlider_valueChanged(int value)
{
    d->setMutationsPerCandidateShape(value);
}

void ImageTaskRunnerWidget::on_randomSeedSpinBox_valueChanged(int value)
{
    d->setRandomSeed(value);
}

void ImageTaskRunnerWidget::on_maxThreadsSpinBox_valueChanged(int value)
{
    d->setMaxThreads(value);
}

void ImageTaskRunnerWidget::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        d->onLanguageChange();
    }
    QWidget::changeEvent(event);
}

}

}
