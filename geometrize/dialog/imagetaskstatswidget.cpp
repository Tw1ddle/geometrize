#include "imagetaskstatswidget.h"
#include "ui_imagetaskstatswidget.h"

#include <cassert>
#include <memory>

#include <QDateTime>
#include <QLocale>
#include <QEvent>

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
        populateUi();
    }
    ~ImageTaskStatsWidgetImpl() = default;
    ImageTaskStatsWidgetImpl operator=(const ImageTaskStatsWidgetImpl&) = delete;
    ImageTaskStatsWidgetImpl(const ImageTaskStatsWidgetImpl&) = delete;

    void setTaskId(const std::size_t id)
    {
        ui->taskIdValueLabel->setText(QLocale().toString(static_cast<uint>(id)));
    }

    void setTimeRunning(const int seconds)
    {
        QDateTime t{QDateTime::fromSecsSinceEpoch(seconds).toUTC()};
        const QString time{QLocale().toString(t, "hh:mm:ss")};
        ui->timeRunningValueLabel->setText(time);
    }

    void setCurrentStatus(const ImageTaskStatsWidget::ImageTaskStatus status)
    {
        m_status = status;
        setCurrentStatusText();
    }

    void setShapeCount(const std::size_t shapeCount)
    {
        ui->shapeCountValueLabel->setText(QLocale().toString(static_cast<uint>(shapeCount)));
    }

    void setSimilarity(const float similarity)
    {
        const QString similarityValue{QLocale().toString(100.0f - similarity, 'f', 2)};
        ui->targetSimilarityValueLabel->setText(similarityValue);
    }

    void setImageDimensions(const std::uint32_t width, const std::uint32_t height)
    {
        m_width = width;
        m_height = height;
        setImageDimensionsText();
    }

    void onLanguageChange()
    {
        ui->retranslateUi(q);
        populateUi();
    }

private:
    void populateUi()
    {
        setImageDimensionsText();
        setCurrentStatusText();
    }

    void setImageDimensionsText()
    {
        const QString imageDimensionsText{tr("%1x%2", "Dimensions of an image e.g. width-x-height, 1024x800").arg(QLocale().toString(static_cast<uint>(m_width))).arg(QLocale().toString(static_cast<uint>(m_height)))};
        ui->imageDimensionsValueLabel->setText(imageDimensionsText);
    }

    void setCurrentStatusText()
    {
        switch(m_status) {
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

    ImageTaskStatsWidget::ImageTaskStatus m_status{ImageTaskStatsWidget::ImageTaskStatus::STOPPED};
    std::uint32_t m_width{0};
    std::uint32_t m_height{0};

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

void ImageTaskStatsWidget::setTimeRunning(const int seconds)
{
    d->setTimeRunning(seconds);
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

void ImageTaskStatsWidget::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        d->onLanguageChange();
    }
    QWidget::changeEvent(event);
}

}

}
