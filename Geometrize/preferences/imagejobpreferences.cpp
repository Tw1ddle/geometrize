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
    ImageJobPreferencesImpl(ImageJobPreferences* pQ) : q{pQ}
    {
    }

    ImageJobPreferencesImpl(ImageJobPreferences* pQ, const std::string& filePath) : q{pQ}
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

    geometrize::ImageRunnerOptions getImageRunnerOptions() const
    {
        return m_options;
    }

    void enableShapeTypes(const geometrize::shapes::ShapeTypes shapes)
    {
        const geometrize::shapes::ShapeTypes oldShapeTypes{m_options.shapeTypes};
        m_options.shapeTypes = static_cast<geometrize::shapes::ShapeTypes>(static_cast<std::uint32_t>(m_options.shapeTypes) | static_cast<std::uint32_t>(shapes));
        emit q->signal_shapeTypesEnabled(oldShapeTypes, shapes);
    }

    void disableShapeTypes(const geometrize::shapes::ShapeTypes shapes)
    {
        const geometrize::shapes::ShapeTypes oldShapeTypes{m_options.shapeTypes};
        m_options.shapeTypes = static_cast<geometrize::shapes::ShapeTypes>(static_cast<std::uint32_t>(m_options.shapeTypes) &~ static_cast<std::uint32_t>(shapes));
        emit q->signal_shapeTypesDisabled(oldShapeTypes, shapes);
    }

    void setShapeAlpha(const std::uint8_t alpha)
    {
        const std::uint8_t oldAlpha{m_options.alpha};
        m_options.alpha = alpha;
        emit q->signal_alphaChanged(oldAlpha, alpha);
    }

    void setCandidateShapeCount(const std::uint32_t shapeCount)
    {
        const std::uint32_t oldCount{m_options.shapeCount};
        m_options.shapeCount = shapeCount;
        emit q->signal_candidateShapeCountChanged(oldCount, shapeCount);
    }

    void setMaxShapeMutations(const std::uint32_t maxMutations)
    {
        const std::uint32_t oldMaxMutations{m_options.maxShapeMutations};
        m_options.maxShapeMutations = maxMutations;
        emit q->signal_maxShapeMutationsChanged(oldMaxMutations, maxMutations);
    }

    void setPasses(const std::uint32_t passes)
    {
        const std::uint32_t oldPasses{m_options.passes};
        m_options.passes = passes;
        emit q->signal_passesChanged(oldPasses, passes);
    }

private:
    serialization::ImageJobPreferencesData m_data;
    geometrize::ImageRunnerOptions m_options;
    ImageJobPreferences* q;
};

ImageJobPreferences::ImageJobPreferences() : d{std::make_unique<ImageJobPreferences::ImageJobPreferencesImpl>(this)}
{
}

ImageJobPreferences::ImageJobPreferences(const std::string& filePath) : d{std::make_unique<ImageJobPreferences::ImageJobPreferencesImpl>(this, filePath)}
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

geometrize::ImageRunnerOptions ImageJobPreferences::getImageRunnerOptions() const
{
    return d->getImageRunnerOptions();
}

void ImageJobPreferences::enableShapeTypes(const geometrize::shapes::ShapeTypes shapes)
{
    d->enableShapeTypes(shapes);
}

void ImageJobPreferences::disableShapeTypes(const geometrize::shapes::ShapeTypes shapes)
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

void ImageJobPreferences::setPasses(const std::uint32_t passes)
{
    d->setPasses(passes);
}

}

}
