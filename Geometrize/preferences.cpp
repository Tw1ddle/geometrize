#include "preferences.h"

namespace geometrize
{

namespace preferences
{

class Preferences::PreferencesImpl
{
public:
    PreferencesImpl()
    {
    }

    ~PreferencesImpl() = default;
    PreferencesImpl& operator=(const PreferencesImpl&) = default;
    PreferencesImpl(const PreferencesImpl&) = default;
};

Preferences::Preferences() : d{std::make_unique<Preferences::PreferencesImpl>()}
{

}

}

}
