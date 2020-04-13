#include "appsplashscreen.h"

#include <cassert>
#include <memory>

#include <QColor>
#include <QCoreApplication>
#include <QPainter>
#include <QProgressBar>
#include <QSplashScreen>
#include <QStyleOptionProgressBar>

#include "common/uiactions.h"
#include "common/util.h"

namespace geometrize
{

namespace dialog
{

class GeometrizeSplashScreen : public QSplashScreen
{
public:
    GeometrizeSplashScreen() : QSplashScreen{nullptr}, m_progress{0}
    {
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
        setPixmap(QPixmap(":/logos/splashscreen.png"));
        setCursor(Qt::BusyCursor);
        setState(SplashState::STARTING);
        if(const auto mouseScreen = geometrize::common::ui::getFirstScreenContainingCursor()) {
            geometrize::common::ui::centerWidgetOnScreen(*this, *mouseScreen);
        }
    }
    virtual ~GeometrizeSplashScreen() = default;

    void setState(const SplashState state)
    {
        switch(state) {
        case SplashState::STARTING:
            setProgress(0);
            show();
            break;
        case SplashState::LOADING_LAUNCHER_WINDOW:
            setProgress(50);
            break;
        case SplashState::FINISHED:
            setProgress(100);
            hide();
            break;
        }

        update();
        processEvents();
    }

private:
    void setProgress(const int value)
    {
        m_progress = geometrize::util::clamp(value, 0, 100);
    }

    virtual void drawContents(QPainter* painter) override
    {
        QSplashScreen::drawContents(painter);

        // Set style for progressbar
        const int barHeight = 24;
        const int border = 0;
        QProgressBar renderer;

        const QString style = "QProgressBar { border: 0px; background-color: #FFFFFF; } QProgressBar::chunk { background-color: #505050; width: 20px; }";

        renderer.resize(width() - border * 2, barHeight);
        renderer.setMinimum(0);
        renderer.setMaximum(100);
        renderer.setValue(m_progress);
        renderer.setTextVisible(false);

        renderer.setStyleSheet(style);
        painter->save();
        painter->translate(border, height() - barHeight);
        renderer.render(painter);
        painter->restore();
    }

    void processEvents()
    {
        QCoreApplication* instance = QCoreApplication::instance();
        if(!instance) {
            assert(0 && "Failed to get app instance, app needs to exist before showing splash screen");
            return;
        }
        instance->processEvents();
    }

    int m_progress;
};

class AppSplashScreen::AppSplashScreenImpl
{
public:
    AppSplashScreenImpl() = default;
    AppSplashScreenImpl& operator=(const AppSplashScreenImpl&) = delete;
    AppSplashScreenImpl(const AppSplashScreenImpl&) = delete;
    ~AppSplashScreenImpl() = default;

    void setState(const SplashState state)
    {
        m_splash.setState(state);
    }

private:
    GeometrizeSplashScreen m_splash;
};

AppSplashScreen::AppSplashScreen() : d{std::make_unique<AppSplashScreen::AppSplashScreenImpl>()}
{
}

AppSplashScreen::~AppSplashScreen()
{
}

void AppSplashScreen::setState(const SplashState state)
{
    d->setState(state);
}

AppSplashScreen& sharedSplashInstance()
{
    static AppSplashScreen instance;
    return instance;
}

}

}
