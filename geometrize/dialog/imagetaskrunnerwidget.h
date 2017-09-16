#pragma once

#include <memory>

#include <QString>
#include <QWidget>

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
 * @brief The ImageTaskRunnerWidget implements a widget for manipulating and changing the settings of an image task e.g. the number of times to mutate each shape the task generates.
 */
class ImageTaskRunnerWidget : public QWidget
{
    Q_OBJECT

public:
    ImageTaskRunnerWidget(QWidget* parent = nullptr);
    ~ImageTaskRunnerWidget();

    /**
     * @brief setImageTask Sets the current image task used by the runner.
     * @param task Non-owning pointer to the image task that the runner settings on this widget will use.
     */
    void setImageTask(task::ImageTask* task);

    /**
     * @brief setRunStopButtonText Sets the text on the run/stop button in the UI.
     * @param text The text to set on the run/stop button.
     */
    void setRunStopButtonText(const QString& text);

    /**
     * @brief syncUserInterface Syncs the user interface with the current image task.
     * This should be called after setting a new image task.
     * Note this does not change the run/stop button text.
     */
    void syncUserInterface();

signals:
    void runStopButtonClicked();
    void stepButtonClicked();
    void clearButtonClicked();

private slots:
    void on_runStopButton_clicked();
    void on_stepButton_clicked();
    void on_clearButton_clicked();

    void on_usesRectangles_clicked(bool checked);
    void on_usesRotatedRectangles_clicked(bool checked);
    void on_usesTriangles_clicked(bool checked);
    void on_usesEllipses_clicked(bool checked);
    void on_usesRotatedEllipses_clicked(bool checked);
    void on_usesCircles_clicked(bool checked);
    void on_usesLines_clicked(bool checked);
    void on_usesQuadraticBeziers_clicked(bool checked);
    void on_usesPolylines_clicked(bool checked);

    void on_shapeOpacitySlider_valueChanged(int value);
    void on_candidateShapesPerStepSlider_valueChanged(int value);
    void on_mutationsPerCandidateShapeSlider_valueChanged(int value);
    void on_randomSeedSpinBox_valueChanged(int value);
    void on_maxThreadsSpinBox_valueChanged(int value);

private:
    class ImageTaskRunnerWidgetImpl;
    std::unique_ptr<ImageTaskRunnerWidgetImpl> d;
};

}

}
