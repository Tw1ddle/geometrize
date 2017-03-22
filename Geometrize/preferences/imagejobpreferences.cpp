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

    ~ImageJobPreferencesImpl() = default;
    ImageJobPreferencesImpl& operator=(const ImageJobPreferencesImpl&) = default;
    ImageJobPreferencesImpl(const ImageJobPreferencesImpl&) = default;

    void load(const std::string& filePath)
    {
        std::ifstream input(filePath);
        try {
            m_data.archive(cereal::JSONInputArchive(input), m_options);
        } catch(...) {
            assert(0 && "Failed to read image preferences");
        }
    }

    void save(const std::string& filePath)
    {
        std::ofstream output(filePath);
        try {
            m_data.archive(cereal::JSONOutputArchive(output), m_options);
        } catch(...) {
            assert(0 && "Failed to write image preferences");
        }
    }

    geometrize::ImageRunnerOptions getImageRunnerOptions() const
    {
        return m_options;
    }

    void enableShapeTypes(const geometrize::ShapeTypes shapes)
    {
        m_options.shapeTypes = static_cast<geometrize::ShapeTypes>(static_cast<std::uint32_t>(m_options.shapeTypes) | static_cast<std::uint32_t>(shapes));
    }

    void disableShapeTypes(const geometrize::ShapeTypes shapes)
    {
        m_options.shapeTypes = static_cast<geometrize::ShapeTypes>(static_cast<std::uint32_t>(m_options.shapeTypes) &~ static_cast<std::uint32_t>(shapes));
    }

    void setShapeAlpha(const std::uint8_t alpha)
    {
        m_options.alpha = alpha;
    }

    void setCandidateShapeCount(const std::uint32_t shapeCount)
    {
        m_options.shapeCount = shapeCount;
    }

    void setMaxShapeMutations(const std::uint32_t maxMutations)
    {
        m_options.maxShapeMutations = maxMutations;
    }

private:
    serialization::ImageJobPreferencesData m_data;
    geometrize::ImageRunnerOptions m_options;
};

ImageJobPreferences::ImageJobPreferences() : d{std::make_shared<ImageJobPreferences::ImageJobPreferencesImpl>()}
{
}

ImageJobPreferences::ImageJobPreferences(const std::string& filePath) : d{std::make_shared<ImageJobPreferences::ImageJobPreferencesImpl>(filePath)}
{
}

ImageJobPreferences::~ImageJobPreferences()
{
}

ImageJobPreferences& ImageJobPreferences::operator=(const ImageJobPreferences& other)
{
    d = other.d;
    return *this;
}

ImageJobPreferences::ImageJobPreferences(const ImageJobPreferences& other) : d{other.d}
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

geometrize::ImageRunnerOptions ImageJobPreferences::getImageRunnerOptions() const
{
    return d->getImageRunnerOptions();
}

void ImageJobPreferences::enableShapeTypes(const geometrize::ShapeTypes shapes)
{
    d->enableShapeTypes(shapes);
}

void ImageJobPreferences::disableShapeTypes(const geometrize::ShapeTypes shapes)
{
    d->disableShapeTypes(shapes);
}

void ImageJobPreferences::setShapeAlpha(const std::uint8_t alpha)
{
    d->setShapeAlpha(alpha);
}

void ImageJobPreferences::setCandidateShapeCount(const std::uint32_t shapeCount)
{
    d->setCandidateShapeCount(shapeCount);
}

void ImageJobPreferences::setMaxShapeMutations(const std::uint32_t maxMutations)
{
    d->setMaxShapeMutations(maxMutations);
}

}

}
