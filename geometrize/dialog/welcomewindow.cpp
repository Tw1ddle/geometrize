#include "welcomewindow.h"
#include "ui_welcomewindow.h"

#include <QDesktopServices>
#include <QEvent>
#include <QUrl>

#include "common/constants.h"
#include "common/uiactions.h"
#include "localization/strings.h"
#include "preferences/globalpreferences.h"

namespace geometrize
{

namespace dialog
{

bool shouldShowWelcomeOnLaunch()
{
    const auto& prefs = geometrize::preferences::getGlobalPreferences();
    return prefs.shouldShowWelcomeScreenOnLaunch();
}

void setShouldShowWelcomeOnLaunch(const bool show)
{
    auto& prefs = geometrize::preferences::getGlobalPreferences();
    prefs.setShouldShowWelcomeScreenOnLaunch(show);
}

WelcomeWindow::WelcomeWindow() :
    ui(new Ui::WelcomeWindow)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    populateUi();

    ui->showOnLaunchCheckbox->setChecked(shouldShowWelcomeOnLaunch());
}

WelcomeWindow::~WelcomeWindow()
{
    delete ui;
}

void WelcomeWindow::on_closeButton_released()
{
    close();
}

void WelcomeWindow::on_videoTutorialsButton_released()
{
    QDesktopServices::openUrl(QUrl(constants::VIDEO_TUTORIAL_URL));
}

void WelcomeWindow::on_showOnLaunchCheckbox_clicked(const bool checked)
{
    setShouldShowWelcomeOnLaunch(checked);
}

void WelcomeWindow::closeEvent (QCloseEvent* event)
{
    geometrize::common::ui::openLaunchWindow();

    QMainWindow::closeEvent(event);
}

void WelcomeWindow::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
        populateUi();
    }
    QMainWindow::changeEvent(event);
}

void WelcomeWindow::populateUi()
{
    const QString welcomeText{tr("Welcome To %1", "A subtitle on the 'tutorial' page welcoming the user to the software").arg(geometrize::strings::Strings::getApplicationName())};

    ui->welcomeTitleLabel->setText(welcomeText);
    setWindowTitle(welcomeText);
}

}

}
