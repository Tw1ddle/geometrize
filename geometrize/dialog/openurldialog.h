#pragma once

#include <QDialog>

class QEvent;

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
class OpenUrlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenUrlDialog(QWidget* parent = nullptr);
    ~OpenUrlDialog();

    /**
     * @brief getUrl Gets the URL the user entered into the dialog.
     * @return The URL the user entered into the dialog.
     */
    QUrl getUrl() const;

protected:
    void changeEvent(QEvent*) override;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    void populateUi();

    Ui::OpenUrlDialog* ui;
};

}

}
