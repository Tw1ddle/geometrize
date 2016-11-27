#ifndef CREDITSDIALOG_H
#define CREDITSDIALOG_H

#include <QDialog>

namespace Ui {
class CreditsDialog;
}

class CreditsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreditsDialog(QWidget *parent = 0);
    ~CreditsDialog();

private:
    Ui::CreditsDialog *ui;
};

#endif // CREDITSDIALOG_H
