#include "imagetaskimagewidget.h"
#include "ui_imagetaskimagewidget.h"

#include <cassert>
#include <memory>

#include <QEvent>
#include <QImage>

#include "common/uiactions.h"
#include "image/imageloader.h"

namespace geometrize
{

namespace dialog
{

class ImageTaskImageWidget::ImageTaskImageWidgetImpl
{
public:
    ImageTaskImageWidgetImpl(ImageTaskImageWidget* pQ) : q{pQ}, ui{std::make_unique<Ui::ImageTaskImageWidget>()}
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

        populateUi();
    }
    ~ImageTaskImageWidgetImpl() = default;
    ImageTaskImageWidgetImpl operator=(const ImageTaskImageWidgetImpl&) = delete;
    ImageTaskImageWidgetImpl(const ImageTaskImageWidgetImpl&) = delete;

    void setTargetImageOpacity(const unsigned int opacity)
    {
        if(static_cast<int>(opacity) == ui->targetImageOpacitySlider->value()) {
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

    void onLanguageChange()
    {
        ui->retranslateUi(q);
        populateUi();
    }

private:
    void populateUi()
    {
    }

    void updateTargetImageOpacity(const unsigned int opacity)
    {
        ui->targetImageOpacitySlider->setValue(opacity);
        ui->targetImageOpacityValueLabel->setText(QString::number(opacity));
    }

    ImageTaskImageWidget* q;
    std::unique_ptr<Ui::ImageTaskImageWidget> ui;
};

ImageTaskImageWidget::ImageTaskImageWidget(QWidget* parent) :
    QWidget{parent},
    d{std::make_unique<ImageTaskImageWidget::ImageTaskImageWidgetImpl>(this)}
{
}

ImageTaskImageWidget::~ImageTaskImageWidget()
{
}

void ImageTaskImageWidget::setTargetImageOpacity(const unsigned int opacity)
{
    d->setTargetImageOpacity(opacity);
}

void ImageTaskImageWidget::setTargetImage(const QImage& image)
{
    d->setTargetImage(image);
}

void ImageTaskImageWidget::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        d->onLanguageChange();
    }
    QWidget::changeEvent(event);
}

}

}
