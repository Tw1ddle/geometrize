#pragma once

#include <QWidget>

class QEvent;

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
    virtual ~FolderSelectionWidget();

signals:

public slots:

protected:
    void changeEvent(QEvent*) override;

private:
    void populateUi();

private:
    Ui::FolderSelectionWidget* ui;
};

}

}
