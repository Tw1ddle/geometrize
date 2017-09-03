#pragma once

#include <vector>

#include <QDialog>

namespace geometrize
{
struct ShapeResult;
}

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
    explicit SvgPreviewDialog(const std::vector<geometrize::ShapeResult>& shapes, QWidget *parent = 0);
    ~SvgPreviewDialog();

private:
    Ui::SvgPreviewDialog *ui;
};

}

}
