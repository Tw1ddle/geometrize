#include "imagejobstatswidget.h"
#include "ui_imagejobstatswidget.h"

#include <cassert>
#include <memory>

#include <QTime>

namespace geometrize
{

namespace dialog
{

class ImageJobStatsWidget::ImageJobStatsWidgetImpl
{
public:
    ImageJobStatsWidgetImpl(ImageJobStatsWidget* pQ) : q{pQ}, ui{std::make_unique<Ui::ImageJobStatsWidget>()}
    {
        ui->setupUi(q);
    }
    ~ImageJobStatsWidgetImpl() = default;
    ImageJobStatsWidgetImpl operator=(const ImageJobStatsWidgetImpl&) = delete;
    ImageJobStatsWidgetImpl(const ImageJobStatsWidgetImpl&) = delete;

    void setTimeRunning(const int millis)
    {
        QTime time(0, 0);
        ui->timeRunningValueLabel->setText(time.addMSecs(millis).toString(Qt::SystemLocaleLongDate));
    }

    void setCurrentStatus(const ImageJobStatsWidget::ImageJobStatus status)
    {
        switch(status) {
        case ImageJobStatsWidget::ImageJobStatus::STOPPED:
            ui->currentStateValueLabel->setText(tr("Idle"));
            break;
        case ImageJobStatsWidget::ImageJobStatus::RUNNING:
            ui->currentStateValueLabel->setText(tr("Running"));
            break;
        default:
            assert(0 && "Unknown image job status set");
            ui->currentStateValueLabel->setText(tr("Unknown"));
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

private:
    ImageJobStatsWidget* q;
    std::unique_ptr<Ui::ImageJobStatsWidget> ui;
};

ImageJobStatsWidget::ImageJobStatsWidget(QWidget* parent) :
    QWidget{parent},
    d{std::make_unique<ImageJobStatsWidget::ImageJobStatsWidgetImpl>(this)}
{
}

ImageJobStatsWidget::~ImageJobStatsWidget()
{
}

void ImageJobStatsWidget::setShapeCount(const std::size_t shapeCount)
{
    d->setShapeCount(shapeCount);
}

void ImageJobStatsWidget::setTimeRunning(const int millis)
{
    d->setTimeRunning(millis);
}

void ImageJobStatsWidget::setCurrentStatus(const ImageJobStatus status)
{
    d->setCurrentStatus(status);
}

void ImageJobStatsWidget::setSimilarity(const float similarity)
{
    d->setSimilarity(similarity);
}

}

}
