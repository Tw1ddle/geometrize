#include "imagejobpreferences.h"

#include <assert.h>
#include <fstream>
#include <ostream>

#include "serialization/imagejobpreferencesdata.h"
#include "cereal/archives/json.hpp"

namespace geometrize
{

namespace preferences
{

class ImageJobPreferences::ImageJobPreferencesImpl
{
public:
    ImageJobPreferencesImpl()
    {
    }

    ImageJobPreferencesImpl(const std::string& filePath)
    {
        load(filePath);
    }

    void load(const std::string& filePath)
    {
        std::ifstream input(filePath);
        try {
            // TODO endsWith
            m_data.load(cereal::JSONInputArchive(input));
        } catch(...) {
            assert(0 && "Failed to read image preferences");
        }
    }

    void save(const std::string& filePath)
    {
        std::ofstream output(filePath);
        try {
            m_data.save(cereal::JSONOutputArchive(output));
        } catch(...) {
            assert(0 && "Failed to write image preferences");
        }
    }

    ~ImageJobPreferencesImpl() = default;
    ImageJobPreferencesImpl& operator=(const ImageJobPreferencesImpl&) = default;
    ImageJobPreferencesImpl(const ImageJobPreferencesImpl&) = default;

private:
    serialization::ImageJobPreferencesData m_data;
};

ImageJobPreferences::ImageJobPreferences() : d{std::make_unique<ImageJobPreferences::ImageJobPreferencesImpl>()} {}

ImageJobPreferences::ImageJobPreferences(const std::string& filePath) : d{std::make_unique<ImageJobPreferences::ImageJobPreferencesImpl>(filePath)}
{
}

ImageJobPreferences::~ImageJobPreferences()
{
}

void ImageJobPreferences::load(const std::string& filePath)
{
    d->load(filePath);
}

void ImageJobPreferences::save(const std::string& filePath)
{
    d->save(filePath);
}

}

}
