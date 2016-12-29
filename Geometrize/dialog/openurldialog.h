#pragma once

#include <QWidget>

namespace Ui
{
class OpenUrlDialog;
}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The OpenUrlDialog class models the UI for the dialog box users can enter URLs into to open remote images or webpages.
 */
class OpenUrlDialog : public QWidget
{
    Q_OBJECT

public:
    explicit OpenUrlDialog(QWidget* parent = 0);
    ~OpenUrlDialog();

    /**
     * @brief getUrl Gets the URL the user entered into the dialog.
     * @return The URL the user entered into the dialog.
     */
    QUrl getUrl() const;

private:
    Ui::OpenUrlDialog* ui;
};

}

}
