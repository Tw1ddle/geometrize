#include "globalpreferences.h"

#include "serialization/globalpreferencesdata.h"

namespace geometrize
{

namespace preferences
{

class GlobalPreferences::GlobalPreferencesImpl
{
public:
    GlobalPreferencesImpl()
    {
        // TODO
        //serialization::load()
    }

    ~GlobalPreferencesImpl() = default;
    GlobalPreferencesImpl& operator=(const GlobalPreferencesImpl&) = default;
    GlobalPreferencesImpl(const GlobalPreferencesImpl&) = default;

    serialization::GlobalPreferencesData m_data;
};

GlobalPreferences::GlobalPreferences() : d{std::make_unique<GlobalPreferences::GlobalPreferencesImpl>()}
{
}

}

}
