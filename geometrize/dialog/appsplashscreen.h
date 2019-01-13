#pragma once

#include <memory>

namespace geometrize
{

namespace dialog
{

/**
 * @brief The SplashState enum provides identifiers for the states of the splash screen.
 */
enum class SplashState
{
    STARTING,
    LOADING_LAUNCHER_WINDOW,
    FINISHED
};

/**
 * @brief The AppSplashScreen class implements a splash screen for Geometrize.
 */
class AppSplashScreen
{
public:
    AppSplashScreen();
    AppSplashScreen& operator=(const AppSplashScreen&) = delete;
    AppSplashScreen(const AppSplashScreen&) = delete;
    ~AppSplashScreen();

    void setState(SplashState state);

private:
    class AppSplashScreenImpl;
    std::unique_ptr<AppSplashScreenImpl> d;
};

/**
 * @brief sharedInstance Gets a singleton/shared instance of the application splash screen.
 * @return A reference to the shared application splash screen instance.
 */
AppSplashScreen& sharedSplashInstance();

}

}
