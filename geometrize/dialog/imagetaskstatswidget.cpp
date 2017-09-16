#include "imagetaskstatswidget.h"
#include "ui_imagetaskstatswidget.h"

#include <cassert>
#include <memory>

#include <QTime>

namespace geometrize
{

namespace dialog
{

class ImageTaskStatsWidget::ImageTaskStatsWidgetImpl
{
public:
    ImageTaskStatsWidgetImpl(ImageTaskStatsWidget* pQ) : q{pQ}, ui{std::make_unique<Ui::ImageTaskStatsWidget>()}
    {
        ui->setupUi(q);
    }
    ~ImageTaskStatsWidgetImpl() = default;
    ImageTaskStatsWidgetImpl operator=(const ImageTaskStatsWidgetImpl&) = delete;
    ImageTaskStatsWidgetImpl(const ImageTaskStatsWidgetImpl&) = delete;

    void setTaskId(std::size_t id)
    {
        ui->taskIdValueLabel->setText(QString::number(id));
    }

    void setTimeRunning(const int millis)
    {
        QTime time(0, 0);
        ui->timeRunningValueLabel->setText(time.addMSecs(millis).toString(Qt::SystemLocaleLongDate));
    }

    void setCurrentStatus(const ImageTaskStatsWidget::ImageTaskStatus status)
    {
        switch(status) {
        case ImageTaskStatsWidget::ImageTaskStatus::STOPPED:
            ui->currentStateValueLabel->setText(tr("Idle", "Text shown on a button when the app is not actively turning images into shapes. That is, the app is idle/not doing anything"));
            break;
        case ImageTaskStatsWidget::ImageTaskStatus::RUNNING:
            ui->currentStateValueLabel->setText(tr("Running", "Text shown on a button when the app is actively turning images into shapes. That is, the app is running in the sense of being busy working/processing"));
            break;
        default:
            assert(0 && "Unknown image task status set");
        }
    }

    void setShapeCount(const std::size_t shapeCount)
    {
        ui->shapeCountValueLabel->setText(QString::number(shapeCount));
    }

    void setSimilarity(const float similarity)
    {
        const QString similarityValue{QString::number(100.0f - similarity, 'f', 2)};
        ui->targetSimilarityValueLabel->setText(similarityValue);
    }

    void setImageDimensions(const std::uint32_t width, const std::uint32_t height)
    {
        const QString imageDimensionsText{tr("%1x%2", "Dimensions of an image e.g. width-x-height, 1024x800").arg(width).arg(height)};
        ui->imageDimensionsValueLabel->setText(imageDimensionsText);
    }

private:
    ImageTaskStatsWidget* q;
    std::unique_ptr<Ui::ImageTaskStatsWidget> ui;
};

ImageTaskStatsWidget::ImageTaskStatsWidget(QWidget* parent) :
    QWidget{parent},
    d{std::make_unique<ImageTaskStatsWidget::ImageTaskStatsWidgetImpl>(this)}
{
}

ImageTaskStatsWidget::~ImageTaskStatsWidget()
{
}

void ImageTaskStatsWidget::setTaskId(const std::size_t id)
{
    d->setTaskId(id);
}

void ImageTaskStatsWidget::setShapeCount(const std::size_t shapeCount)
{
    d->setShapeCount(shapeCount);
}

void ImageTaskStatsWidget::setTimeRunning(const int millis)
{
    d->setTimeRunning(millis);
}

void ImageTaskStatsWidget::setCurrentStatus(const ImageTaskStatus status)
{
    d->setCurrentStatus(status);
}

void ImageTaskStatsWidget::setSimilarity(const float similarity)
{
    d->setSimilarity(similarity);
}

void ImageTaskStatsWidget::setImageDimensions(const std::uint32_t width, const std::uint32_t height)
{
    d->setImageDimensions(width, height);
}

}

}
