#pragma once

#include <memory>

#include <QMainWindow>

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
 * @brief The ImageJobWindow class encapsulates the UI for manipiulating, running and viewing an image job that geometrizes an image.
 */
class ImageJobWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageJobWindow();
    ~ImageJobWindow();

    /**
     * @brief setImageJob Sets the current image job shown by the window.
     * @param job The image job to set on the image job window. The window takes ownership of the image job.
     */
    void setImageJob(job::ImageJob* job);

private slots:
    void on_actionExit_triggered();
    void on_actionLoad_Settings_Template_triggered();
    void on_actionSave_Settings_Template_triggered();
    void on_actionReveal_Launch_Window_triggered();
    void on_actionReveal_Script_Editor_triggered();
    void on_actionScript_Console_toggled(bool checked);
    void on_runStopButton_clicked();
    void on_stepButton_clicked();
    void on_clearButton_clicked();

    void on_saveImageButton_clicked();
    void on_saveImagesButton_clicked();
    void on_previewSVGButton_clicked();
    void on_saveSVGButton_clicked();
    void on_saveGeometryDataButton_clicked();
    void on_saveGIFButton_clicked();
    void on_saveCanvasAnimationButton_clicked();
    void on_saveWebGLButton_clicked();

    void on_usesRectangles_clicked(bool checked);
    void on_usesRotatedRectangles_clicked(bool checked);
    void on_usesTriangles_clicked(bool checked);
    void on_usesEllipses_clicked(bool checked);
    void on_usesRotatedEllipses_clicked(bool checked);
    void on_usesCircles_clicked(bool checked);
    void on_usesLines_clicked(bool checked);
    void on_usesQuadraticBeziers_clicked(bool checked);
    void on_usesPolylines_clicked(bool checked);

    void on_scriptingModeEnabledCheckbox_clicked(bool checked);
    void on_resetShapeScriptEngineButton_clicked();

    void on_shapeOpacitySlider_valueChanged(int value);
    void on_candidateShapesPerStepSlider_valueChanged(int value);
    void on_mutationsPerCandidateShapeSlider_valueChanged(int value);
    void on_randomSeedSpinBox_valueChanged(int value);
    void on_maxThreadsSpinBox_valueChanged(int value);

private:
    class ImageJobWindowImpl;
    std::unique_ptr<ImageJobWindowImpl> d;
};

}

}
