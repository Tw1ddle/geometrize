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
    void on_imageResizeCheckbox_toggled(bool checked);
    void on_resizeWidthSpinBox_valueChanged(int value);
    void on_resizeHeightSpinBox_valueChanged(int value);

    void on_maxThreadsPerImageJobSpinBox_valueChanged(int value);

private:
    class GlobalPreferencesDialogImpl;
    std::unique_ptr<GlobalPreferencesDialogImpl> d;
};

}

}
