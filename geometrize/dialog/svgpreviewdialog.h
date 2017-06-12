#pragma once

#include <QDialog>

namespace Ui
{
class SvgPreviewDialog;
}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The SvgPreviewDialog class provides a dialog for previewing SVGs.
 */
class SvgPreviewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SvgPreviewDialog(QWidget *parent = 0);
    ~SvgPreviewDialog();

private:
    Ui::SvgPreviewDialog *ui;
};

}

}
