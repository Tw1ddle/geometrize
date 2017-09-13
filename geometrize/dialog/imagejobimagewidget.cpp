#include "imagejobimagewidget.h"
#include "ui_imagejobimagewidget.h"

#include <cassert>
#include <memory>

#include <QImage>

#include "common/uiactions.h"
#include "image/imageloader.h"

namespace geometrize
{

namespace dialog
{

class ImageJobImageWidget::ImageJobImageWidgetImpl
{
public:
    ImageJobImageWidgetImpl(ImageJobImageWidget* pQ) : q{pQ}, ui{std::make_unique<Ui::ImageJobImageWidget>()}
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
    ~ImageJobImageWidgetImpl() = default;
    ImageJobImageWidgetImpl operator=(const ImageJobImageWidgetImpl&) = delete;
    ImageJobImageWidgetImpl(const ImageJobImageWidgetImpl&) = delete;

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

    ImageJobImageWidget* q;
    std::unique_ptr<Ui::ImageJobImageWidget> ui;
};

ImageJobImageWidget::ImageJobImageWidget(QWidget* parent) :
    QWidget{parent},
    d{std::make_unique<ImageJobImageWidget::ImageJobImageWidgetImpl>(this)}
{
}

ImageJobImageWidget::~ImageJobImageWidget()
{
}

void ImageJobImageWidget::setTargetImageOpacity(const unsigned int opacity)
{
    d->setTargetImageOpacity(opacity);
}

void ImageJobImageWidget::setTargetImage(const QImage& image)
{
    d->setTargetImage(image);
}

}

}
