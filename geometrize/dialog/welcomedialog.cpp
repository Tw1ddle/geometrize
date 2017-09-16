#include "welcomedialog.h"
#include "ui_welcomedialog.h"

#include <QDesktopServices>
#include <QUrl>

#include "common/constants.h"
#include "localization/strings.h"
#include "preferences/globalpreferences.h"

namespace geometrize
{

namespace dialog
{

bool shouldShowWelcomeOnLaunch()
{
    auto& prefs{geometrize::preferences::getGlobalPreferences()};
    return prefs.shouldShowWelcomeScreenOnLaunch();
}

void setShouldShowWelcomeOnLaunch(const bool show)
{
    auto& prefs{geometrize::preferences::getGlobalPreferences()};
    prefs.setShouldShowWelcomeScreenOnLaunch(show);
}

WelcomeDialog::WelcomeDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::WelcomeDialog)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint); // Remove question mark from title bar
    ui->setupUi(this);

    ui->welcomeTitleLabel->setText(tr("Welcome To %1", "A subtitle on the 'tutorial' page welcoming the user to the software").arg(geometrize::strings::Strings::getApplicationName()));

    ui->showOnLaunchCheckbox->setChecked(shouldShowWelcomeOnLaunch());
}

WelcomeDialog::~WelcomeDialog()
{
    delete ui;
}

void WelcomeDialog::on_closeButton_released()
{
    close();
}

void WelcomeDialog::on_videoTutorialsButton_released()
{
    QDesktopServices::openUrl(QUrl(constants::VIDEO_TUTORIAL_URL));
}

void WelcomeDialog::on_showOnLaunchCheckbox_clicked(const bool checked)
{
    setShouldShowWelcomeOnLaunch(checked);
}

}

}
