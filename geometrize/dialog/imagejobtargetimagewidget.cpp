#include "imagejobtargetimagewidget.h"
#include "ui_imagejobtargetimagewidget.h"

#include <cassert>
#include <memory>

#include <QImage>

#include "common/uiactions.h"
#include "image/imageloader.h"

namespace geometrize
{

namespace dialog
{

class ImageJobTargetImageWidget::ImageJobTargetImageWidgetImpl
{
public:
    ImageJobTargetImageWidgetImpl(ImageJobTargetImageWidget* pQ) : q{pQ}, ui{std::make_unique<Ui::ImageJobTargetImageWidget>()}
    {
        ui->setupUi(q);

        q->connect(ui->targetImageOpacitySlider, &QSlider::valueChanged, [this](int value) {
            updateTargetImageOpacity(static_cast<unsigned int>(value));
            emit q->targetImageOpacityChanged(static_cast<unsigned int>(value));
        });

        q->connect(ui->pickTargetImageButton, &QPushButton::clicked, [this]() {
            const QString imagePath{geometrize::common::ui::openTargetImagePickerDialog(q)};
            if(imagePath.isEmpty()) {
                return;
            }

            const QImage image{geometrize::image::loadImage(imagePath.toStdString())};
            if(image.isNull()) {
                return;
            }

            emit q->targetImageSelected(image);
        });
    }
    ~ImageJobTargetImageWidgetImpl() = default;
    ImageJobTargetImageWidgetImpl operator=(const ImageJobTargetImageWidgetImpl&) = delete;
    ImageJobTargetImageWidgetImpl(const ImageJobTargetImageWidgetImpl&) = delete;

    void setTargetImageOpacity(const unsigned int opacity)
    {
        if(opacity == ui->targetImageOpacitySlider->value()) {
            return;
        }
        updateTargetImageOpacity(opacity);
        emit q->targetImageOpacityChanged(opacity);
    }

    void setTargetImage(const QImage& image)
    {
        assert(!image.isNull() && "Attempting to set a bad target image");

        const int thumbnailSize{400};
        ui->targetImageLabel->setPixmap(QPixmap::fromImage(image.scaled(thumbnailSize, thumbnailSize, Qt::KeepAspectRatio)));

        emit q->targetImageSet(image);
    }

private:
    void updateTargetImageOpacity(const unsigned int opacity)
    {
        ui->targetImageOpacitySlider->setValue(opacity);
        ui->targetImageOpacityValueLabel->setText(QString::number(opacity));
    }

    ImageJobTargetImageWidget* q;
    std::unique_ptr<Ui::ImageJobTargetImageWidget> ui;
};

ImageJobTargetImageWidget::ImageJobTargetImageWidget(QWidget* parent) :
    QWidget{parent},
    d{std::make_unique<ImageJobTargetImageWidget::ImageJobTargetImageWidgetImpl>(this)}
{
}

ImageJobTargetImageWidget::~ImageJobTargetImageWidget()
{
}

void ImageJobTargetImageWidget::setTargetImageOpacity(const unsigned int opacity)
{
    d->setTargetImageOpacity(opacity);
}

void ImageJobTargetImageWidget::setTargetImage(const QImage& image)
{
    d->setTargetImage(image);
}

}

}
