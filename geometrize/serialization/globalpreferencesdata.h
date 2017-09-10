#pragma once

#include <string>
#include <utility>

#include "cereal/cereal.hpp"
#include "cereal/types/utility.hpp"

namespace geometrize
{

namespace serialization
{

/**
 * @brief The GlobalPreferencesData struct represents the data for the global application preferences.
 */
class GlobalPreferencesData
{
public:
    template<class Archive>
    void archive(Archive& ar,
                 bool& shouldShowWelcomeScreenOnLaunch,
                 bool& imageJobImageResizeEnabled,
                 std::pair<std::uint32_t, std::uint32_t>& imageJobImageResizeThreshold,
                 std::string& isoLanguageCode,
                 std::string& isoScriptCode,
                 std::string& isoCountryCode)
    {
        ar(cereal::make_nvp(shouldShowWelcomeScreenOnLaunchKey, shouldShowWelcomeScreenOnLaunch));
        ar(cereal::make_nvp(imageJobImageResizeEnabledKey, imageJobImageResizeEnabled));
        ar(cereal::make_nvp(imageJobImageResizeThresholdKey, imageJobImageResizeThreshold));
        ar(cereal::make_nvp(isoLanguageCodeKey, isoLanguageCode));
        ar(cereal::make_nvp(isoScriptCodeKey, isoScriptCode));
        ar(cereal::make_nvp(isoCountryCodeKey, isoCountryCode));
    }

private:
    const std::string shouldShowWelcomeScreenOnLaunchKey{"shouldShowWelcomeScreenOnLaunch"};
    const std::string imageJobImageResizeEnabledKey{"imageJobImageResizeEnabled"};
    const std::string imageJobImageResizeThresholdKey{"imageJobImageResizeThreshold"};
    const std::string isoLanguageCodeKey{"isoLanguageCode"};
    const std::string isoScriptCodeKey{"isoScriptCode"};
    const std::string isoCountryCodeKey{"isoCountryCode"};
};

}

}
