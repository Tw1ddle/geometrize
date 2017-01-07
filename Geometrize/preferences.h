#pragma once

#include <memory>

#include <QObject>

namespace geometrize
{

namespace preferences
{

/**
 * @brief The Preferences class models the preferences associated with the application as a whole.
 * These mostly represent application-wide settings that are edited via the tabbed preferences widget.
 */
class Preferences : public QObject
{
    Q_OBJECT
public:
    Preferences();
    Preferences& operator=(const Preferences&) = delete;
    Preferences(const Preferences&) = delete;
    ~Preferences() = default;

private:
    class PreferencesImpl;
    std::unique_ptr<PreferencesImpl> d;
};

}

}
