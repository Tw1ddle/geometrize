#pragma once

#include <cstdint>
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

                 bool& shouldGeometrizeAppLogoOnLaunch,
                 bool& shouldPopulateRecentItemsOnLaunch,
                 bool& shouldPopulateTemplatesOnLaunch,

                 bool& shouldShowLaunchConsoleByDefault,
                 bool& shouldShowImageJobVectorViewByDefault,
                 bool& shouldShowImageJobConsoleByDefault,
                 bool& shouldShowImageJobScriptEditorByDefault,
                 bool& shouldShowImageJobPixmapViewByDefault,

                 bool& imageJobImageResizeEnabled,
                 std::pair<std::uint32_t, std::uint32_t>& imageJobImageResizeThreshold,
                 std::uint32_t& imageJobMaxThreads,

                 std::string& isoLanguageCode,
                 std::string& isoScriptCode,
                 std::string& isoCountryCode)
    {
        ar(cereal::make_nvp(shouldShowWelcomeScreenOnLaunchKey, shouldShowWelcomeScreenOnLaunch));

        ar(cereal::make_nvp(shouldGeometrizeAppLogoOnLaunchKey, shouldGeometrizeAppLogoOnLaunch));
        ar(cereal::make_nvp(shouldPopulateRecentItemsOnLaunchKey, shouldPopulateRecentItemsOnLaunch));
        ar(cereal::make_nvp(shouldPopulateTemplatesOnLaunchKey, shouldPopulateTemplatesOnLaunch));

        ar(cereal::make_nvp(shouldShowLaunchConsoleByDefaultKey, shouldShowLaunchConsoleByDefault));
        ar(cereal::make_nvp(shouldShowImageJobVectorViewByDefaultKey, shouldShowImageJobVectorViewByDefault));
        ar(cereal::make_nvp(shouldShowImageJobConsoleByDefaultKey, shouldShowImageJobConsoleByDefault));
        ar(cereal::make_nvp(shouldShowImageJobScriptEditorByDefaultKey, shouldShowImageJobScriptEditorByDefault));
        ar(cereal::make_nvp(shouldShowImageJobPixmapViewByDefaultKey, shouldShowImageJobPixmapViewByDefault));

        ar(cereal::make_nvp(imageJobImageResizeEnabledKey, imageJobImageResizeEnabled));
        ar(cereal::make_nvp(imageJobImageResizeThresholdKey, imageJobImageResizeThreshold));
        ar(cereal::make_nvp(imageJobMaxThreadsKey, imageJobMaxThreads));

        ar(cereal::make_nvp(isoLanguageCodeKey, isoLanguageCode));
        ar(cereal::make_nvp(isoScriptCodeKey, isoScriptCode));
        ar(cereal::make_nvp(isoCountryCodeKey, isoCountryCode));
    }

private:
    const std::string shouldShowWelcomeScreenOnLaunchKey{"shouldShowWelcomeScreenOnLaunch"};

    const std::string shouldGeometrizeAppLogoOnLaunchKey{"shouldGeometrizeAppLogoOnLaunch"};
    const std::string shouldPopulateRecentItemsOnLaunchKey{"shouldPopulateRecentItemsOnLaunch"};
    const std::string shouldPopulateTemplatesOnLaunchKey{"shouldPopulateTemplatesOnLaunch"};

    const std::string shouldShowLaunchConsoleByDefaultKey{"shouldShowLaunchConsoleByDefault"};
    const std::string shouldShowImageJobVectorViewByDefaultKey{"shouldShowImageJobVectorViewByDefault"};
    const std::string shouldShowImageJobConsoleByDefaultKey{"shouldShowImageJobConsoleByDefault"};
    const std::string shouldShowImageJobScriptEditorByDefaultKey{"shouldShowImageJobScriptEditorByDefault"};
    const std::string shouldShowImageJobPixmapViewByDefaultKey{"shouldShowImageJobPixmapViewByDefault"};

    const std::string imageJobImageResizeEnabledKey{"imageJobImageResizeEnabled"};
    const std::string imageJobImageResizeThresholdKey{"imageJobImageResizeThreshold"};
    const std::string imageJobMaxThreadsKey{"imageJobMaxThreadsKey"};

    const std::string isoLanguageCodeKey{"isoLanguageCode"};
    const std::string isoScriptCodeKey{"isoScriptCode"};
    const std::string isoCountryCodeKey{"isoCountryCode"};
};

}

}
