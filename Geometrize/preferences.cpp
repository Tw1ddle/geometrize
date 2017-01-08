#include "preferences.h"

#include "serialization/preferencesdata.h"

namespace geometrize
{

namespace preferences
{

class Preferences::PreferencesImpl
{
public:
    PreferencesImpl()
    {
        // TODO
        //serialization::load()
    }

    ~PreferencesImpl() = default;
    PreferencesImpl& operator=(const PreferencesImpl&) = default;
    PreferencesImpl(const PreferencesImpl&) = default;

    serialization::PreferencesData m_data;
};

Preferences::Preferences() : d{std::make_unique<Preferences::PreferencesImpl>()}
{
}

}

}
