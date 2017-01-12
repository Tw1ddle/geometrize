#pragma once

#include <memory>

#include <QObject>

namespace geometrize
{

namespace preferences
{

/**
 * @brief The GlobalPreferences class models the preferences associated with the application as a whole.
 * These mostly represent application-wide settings that are edited via the tabbed preferences widget.
 */
class GlobalPreferences : public QObject
{
    Q_OBJECT
public:
    GlobalPreferences();
    GlobalPreferences& operator=(const GlobalPreferences&) = delete;
    GlobalPreferences(const GlobalPreferences&) = delete;
    ~GlobalPreferences() = default;

private:
    class GlobalPreferencesImpl;
    std::unique_ptr<GlobalPreferencesImpl> d;
};

}

}
