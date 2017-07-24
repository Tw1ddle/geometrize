#include "imagejobpreferences.h"

#include <cassert>
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
            cereal::JSONInputArchive archive{input};
            m_data.archive(archive, m_options, m_scriptsEnabled, m_scripts);
        } catch(...) {
            assert(0 && "Failed to read image preferences");
        }
    }

    void save(const std::string& filePath)
    {
        std::ofstream output(filePath);
        try {
            cereal::JSONOutputArchive archive{output};
            m_data.archive(archive, m_options, m_scriptsEnabled, m_scripts);
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

    void setShapeTypes(const geometrize::ShapeTypes shapes)
    {
        m_options.shapeTypes = shapes;
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

    void setSeed(const std::uint32_t seed)
    {
        m_options.seed = seed;
    }

    void setMaxThreads(const std::uint32_t maxThreads)
    {
        m_options.maxThreads = maxThreads;
    }

    void setScriptModeEnabled(const bool enabled)
    {
        m_scriptsEnabled = enabled;
    }

    bool isScriptModeEnabled() const
    {
        return m_scriptsEnabled;
    }

    void setScript(const std::string& scriptName, const std::string& code)
    {
        m_scripts[scriptName] = code;
    }

    std::map<std::string, std::string> getScripts() const
    {
        return m_scripts;
    }

private:
    serialization::ImageJobPreferencesData m_data; ///> The data that will be serialized/deserialized
    geometrize::ImageRunnerOptions m_options; ///> The Geometrize library-level image runner options

    bool m_scriptsEnabled{false}; ///> Whether the custom Chaiscript scripts are enabled or not
    std::map<std::string, std::string> m_scripts; ///> Custom Chaiscript scripts that override the default Geometrize functionality
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

void ImageJobPreferences::setShapeTypes(const geometrize::ShapeTypes shapes)
{
    d->setShapeTypes(shapes);
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

void ImageJobPreferences::setSeed(const std::uint32_t seed)
{
    d->setSeed(seed);
}

void ImageJobPreferences::setMaxThreads(const std::uint32_t maxThreads)
{
    d->setMaxThreads(maxThreads);
}

void ImageJobPreferences::setScriptModeEnabled(const bool enabled)
{
    d->setScriptModeEnabled(enabled);
}

bool ImageJobPreferences::isScriptModeEnabled() const
{
    return d->isScriptModeEnabled();
}

void ImageJobPreferences::setScript(const std::string& scriptName, const std::string& code)
{
    d->setScript(scriptName, code);
}

std::map<std::string, std::string> ImageJobPreferences::getScripts() const
{
    return d->getScripts();
}

}

}
