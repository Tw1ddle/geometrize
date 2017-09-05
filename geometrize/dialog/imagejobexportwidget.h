#pragma once

#include <memory>
#include <vector>

#include <QWidget>

namespace geometrize
{
struct ShapeResult;
}

namespace geometrize
{

namespace job
{
class ImageJob;
}

}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The ImageJobExportWidget implements a widget for accessing and using the image job exporters e.g. for exporting image job shape data to JSON.
 */
class ImageJobExportWidget : public QWidget
{
    Q_OBJECT

public:
    ImageJobExportWidget(QWidget* parent = nullptr);
    ~ImageJobExportWidget();

    /**
     * @brief setImageJob Sets the current image job used by the export functions.
     * @param job Non-owning pointer to the image job that the exporters on this widget will use.
     * @param shapes A non-owning pointer to the shape data produced by the image job.
     */
    void setImageJob(const job::ImageJob* job, const std::vector<geometrize::ShapeResult>* shapes);

private slots:
    void on_saveImageButton_clicked();
    void on_saveImagesButton_clicked();
    void on_saveSVGButton_clicked();
    void on_saveGeometryDataButton_clicked();
    void on_saveGIFButton_clicked();
    void on_saveHTML5WebpageButton_clicked();
    void on_saveWebGLWebpageButton_clicked();

private:
    class ImageJobExportWidgetImpl;
    std::unique_ptr<ImageJobExportWidgetImpl> d;
};

}

}
