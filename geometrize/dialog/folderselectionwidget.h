#pragma once

#include <QWidget>

namespace Ui
{
class FolderSelectionWidget;
}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The FolderSelectionWidget class implements a widget through which a folder can be selected, and displays the last selected folder path.
 */
class FolderSelectionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FolderSelectionWidget(QWidget* parent = nullptr);
    ~FolderSelectionWidget();

signals:

public slots:

private:
    Ui::FolderSelectionWidget* ui;
};

}

}
