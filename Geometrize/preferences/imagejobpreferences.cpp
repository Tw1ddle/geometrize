#include "imagejobpreferences.h"

#include <assert.h>
#include <fstream>

#include "serialization/imagejobpreferencesdata.h"
#include "cereal/archives/json.hpp"

namespace geometrize
{

namespace preferences
{

class ImageJobPreferences::ImageJobPreferencesImpl
{
public:
    ImageJobPreferencesImpl(const std::string& manifestFilepath)
    {
        std::ifstream input(manifestFilepath);
        try {
            m_data.load(cereal::JSONInputArchive(input));
        } catch(...) {
            assert(0 && "Failed to read template manifest");
        }
    }
    ~ImageJobPreferencesImpl() = default;
    ImageJobPreferencesImpl& operator=(const ImageJobPreferencesImpl&) = default;
    ImageJobPreferencesImpl(const ImageJobPreferencesImpl&) = default;

    serialization::ImageJobPreferencesData m_data;
};

ImageJobPreferences::ImageJobPreferences() {}

ImageJobPreferences::ImageJobPreferences(const std::string& manifestFilepath) : d{std::make_unique<ImageJobPreferences::ImageJobPreferencesImpl>(manifestFilepath)}
{
}

ImageJobPreferences::~ImageJobPreferences()
{
}

}

}
