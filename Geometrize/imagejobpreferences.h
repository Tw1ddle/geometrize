#pragma once

#include <memory>
#include <string>

namespace geometrize
{

namespace preferences
{

/**
 * @brief The ImageJobPreferences class models the preferences associated with an image job.
 */
class ImageJobPreferences
{
public:
    ImageJobPreferences();
    ImageJobPreferences(const std::string& settingsFilepath);
    ImageJobPreferences& operator=(const ImageJobPreferences&) = delete;
    ImageJobPreferences(const ImageJobPreferences&) = delete;
    ~ImageJobPreferences();

private:
    class ImageJobPreferencesImpl;
    std::unique_ptr<ImageJobPreferencesImpl> d;
};

}

}
