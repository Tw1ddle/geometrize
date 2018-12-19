#pragma once

#include <memory>
#include <vector>

#include <QWidget>

class QEvent;

namespace geometrize
{
struct ShapeResult;
}

namespace geometrize
{

namespace task
{
class ImageTask;
}

}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The ImageTaskExportWidget implements a widget for accessing and using the image task exporters e.g. for exporting image task shape data to JSON.
 */
class ImageTaskExportWidget : public QWidget
{
    Q_OBJECT

public:
    ImageTaskExportWidget(QWidget* parent = nullptr);
    virtual ~ImageTaskExportWidget();

    /**
     * @brief setImageTask Sets the current image task used by the export functions.
     * @param task Non-owning pointer to the image task that the exporters on this widget will use.
     * @param shapes A non-owning pointer to the shape data produced by the image task.
     */
    void setImageTask(const task::ImageTask* task, const std::vector<geometrize::ShapeResult>* shapes);

protected:
    void changeEvent(QEvent*) override;

private slots:
    void on_saveImageButton_clicked();
    void on_saveImagesButton_clicked();
    void on_saveSVGButton_clicked();
    void on_saveGeometryDataButton_clicked();
    void on_saveGIFButton_clicked();
    void on_saveHTML5WebpageButton_clicked();
    void on_saveWebGLWebpageButton_clicked();

private:
    class ImageTaskExportWidgetImpl;
    std::unique_ptr<ImageTaskExportWidgetImpl> d;
};

}

}
