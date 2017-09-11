#pragma once

#include <memory>

#include <QCloseEvent>
#include <QDialog>

namespace geometrize
{

namespace dialog
{

/**
 * @brief The GlobalPreferencesDialog class models the UI for a tabbed preferences dialog for application-level preferences.
 * Note this is a modal dialog, job-specific preferences are edited elsewhere.
 */
class GlobalPreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GlobalPreferencesDialog(QWidget* parent = nullptr);
    ~GlobalPreferencesDialog();

private slots:    
    void on_geometrizeLogo_toggled(bool checked);
    void on_populateRecents_toggled(bool checked);
    void on_populateTemplates_toggled(bool checked);

    void on_showLaunchWindowConsoleByDefault_toggled(bool checked);
    void on_showVectorResultsByDefault_toggled(bool checked);
    void on_showPixmapResultsByDefault_toggled(bool checked);
    void on_showImageJobConsoleByDefault_toggled(bool checked);
    void on_showScriptEditorsByDefault_toggled(bool checked);

    void on_imageResize_toggled(bool checked);
    void on_resizeWidth_valueChanged(int value);
    void on_resizeHeight_valueChanged(int value);
    void on_maxThreadsPerImageJob_valueChanged(int value);

private:
    class GlobalPreferencesDialogImpl;
    std::unique_ptr<GlobalPreferencesDialogImpl> d;
};

}

}
