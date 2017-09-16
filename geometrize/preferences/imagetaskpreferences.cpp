#include "imagetaskpreferences.h"

#include <cassert>
#include <fstream>
#include <ostream>

#include "serialization/imagetaskpreferencesdata.h"
#include "cereal/archives/json.hpp"

namespace geometrize
{

namespace preferences
{

class ImageTaskPreferences::ImageTaskPreferencesImpl
{
public:
    ImageTaskPreferencesImpl()
    {
    }

    ImageTaskPreferencesImpl(const std::string& filePath)
    {
        load(filePath);
    }

    ~ImageTaskPreferencesImpl() = default;
    ImageTaskPreferencesImpl& operator=(const ImageTaskPreferencesImpl&) = default;
    ImageTaskPreferencesImpl(const ImageTaskPreferencesImpl&) = default;

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
    serialization::ImageTaskPreferencesData m_data; ///> The data that will be serialized/deserialized
    geometrize::ImageRunnerOptions m_options; ///> The Geometrize library-level image runner options

    bool m_scriptsEnabled{false}; ///> Whether the custom Chaiscript scripts are enabled or not
    std::map<std::string, std::string> m_scripts; ///> Custom Chaiscript scripts that override the default Geometrize functionality
};

ImageTaskPreferences::ImageTaskPreferences() : d{std::make_shared<ImageTaskPreferences::ImageTaskPreferencesImpl>()}
{
}

ImageTaskPreferences::ImageTaskPreferences(const std::string& filePath) : d{std::make_shared<ImageTaskPreferences::ImageTaskPreferencesImpl>(filePath)}
{
}

ImageTaskPreferences::~ImageTaskPreferences()
{
}

ImageTaskPreferences& ImageTaskPreferences::operator=(const ImageTaskPreferences& other)
{
    d = other.d;
    return *this;
}

ImageTaskPreferences::ImageTaskPreferences(const ImageTaskPreferences& other) : d{other.d}
{
}

void ImageTaskPreferences::load(const std::string& filePath)
{
    d->load(filePath);
}

void ImageTaskPreferences::save(const std::string& filePath)
{
    d->save(filePath);
}

geometrize::ImageRunnerOptions ImageTaskPreferences::getImageRunnerOptions() const
{
    return d->getImageRunnerOptions();
}

void ImageTaskPreferences::enableShapeTypes(const geometrize::ShapeTypes shapes)
{
    d->enableShapeTypes(shapes);
}

void ImageTaskPreferences::disableShapeTypes(const geometrize::ShapeTypes shapes)
{
    d->disableShapeTypes(shapes);
}

void ImageTaskPreferences::setShapeTypes(const geometrize::ShapeTypes shapes)
{
    d->setShapeTypes(shapes);
}

void ImageTaskPreferences::setShapeAlpha(const std::uint8_t alpha)
{
    d->setShapeAlpha(alpha);
}

void ImageTaskPreferences::setCandidateShapeCount(const std::uint32_t shapeCount)
{
    d->setCandidateShapeCount(shapeCount);
}

void ImageTaskPreferences::setMaxShapeMutations(const std::uint32_t maxMutations)
{
    d->setMaxShapeMutations(maxMutations);
}

void ImageTaskPreferences::setSeed(const std::uint32_t seed)
{
    d->setSeed(seed);
}

void ImageTaskPreferences::setMaxThreads(const std::uint32_t maxThreads)
{
    d->setMaxThreads(maxThreads);
}

void ImageTaskPreferences::setScriptModeEnabled(const bool enabled)
{
    d->setScriptModeEnabled(enabled);
}

bool ImageTaskPreferences::isScriptModeEnabled() const
{
    return d->isScriptModeEnabled();
}

void ImageTaskPreferences::setScript(const std::string& scriptName, const std::string& code)
{
    d->setScript(scriptName, code);
}

std::map<std::string, std::string> ImageTaskPreferences::getScripts() const
{
    return d->getScripts();
}

}

}
