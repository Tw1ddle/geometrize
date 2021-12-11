#include "imagetaskpreferences.h"

#include <cassert>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>

#include <QDir>
#include <QFileInfo>

#include "cereal/archives/json.hpp"

#include "common/searchpaths.h"
#include "serialization/imagetaskpreferencesdata.h"
#include "serialization/streamview.h"

namespace geometrize
{

namespace preferences
{

std::string getImageTaskPreferencesAutoSavePath(const std::string& hashOfFirstTargetImage)
{
    const std::vector<std::string> autoSaveSearchPaths = geometrize::searchpaths::getAutosaveTaskSettingsSearchPaths();
    if(autoSaveSearchPaths.empty()) {
        assert(0 && "Image task autosave search paths should never be empty");
        return "";
    }
    const auto autoSaveTaskSettingsFilename = geometrize::searchpaths::getAutosaveTaskSettingsFilename(hashOfFirstTargetImage);
    return autoSaveSearchPaths[0] + QDir::separator().toLatin1() + autoSaveTaskSettingsFilename;
}

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

    bool load(const std::string& filePath)
    {
        // Preferences can be bundled into Qt resources, so we use a streamview that loads the file contents into a byte array first
        serialization::StreamView streamView(filePath);
        std::istream input(&streamView);
        try {
            cereal::JSONInputArchive archive{input};
            m_data.archive(archive, m_options, m_scriptsEnabled, m_scripts);
        } catch(...) {
            return false;
        }
        return true;
    }

    bool save(const std::string& filePath)
    {
        // Create the directory at the filepath if it does not already exist
        // This is necessary because the output archive will not create any missing directories
        const QFileInfo info(QString::fromStdString(filePath));
        const QDir dir(info.absoluteDir());
        if(!dir.exists() && !dir.mkpath(dir.absolutePath())) {
            assert(0 && "Failed to create directory in which to save image task preferences");
            return false;
        }

        std::ofstream output(filePath);
        try {
            cereal::JSONOutputArchive archive{output};
            m_data.archive(archive, m_options, m_scriptsEnabled, m_scripts);
        } catch(...) {
            assert(0 && "Failed to write image task preferences");
            return false;
        }
        return true;
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

    void setShapeBounds(const geometrize::ImageRunnerShapeBoundsOptions& shapeBounds)
    {
        m_options.shapeBounds = shapeBounds;
    }

    void setShapeBounds(const double xMinPercent, const double yMinPercent, const double xMaxPercent, const double yMaxPercent)
    {
        m_options.shapeBounds.xMinPercent = xMinPercent;
        m_options.shapeBounds.yMinPercent = yMinPercent;
        m_options.shapeBounds.xMaxPercent = xMaxPercent;
        m_options.shapeBounds.yMaxPercent = yMaxPercent;
    }

    void setShapeBoundsXMinPercent(const double xMinPercent)
    {
        m_options.shapeBounds.xMinPercent = xMinPercent;
    }

    void setShapeBoundsYMinPercent(const double yMinPercent)
    {
        m_options.shapeBounds.yMinPercent = yMinPercent;
    }

    void setShapeBoundsXMaxPercent(const double xMaxPercent)
    {
        m_options.shapeBounds.xMaxPercent = xMaxPercent;
    }

    void setShapeBoundsYMaxPercent(const double yMaxPercent)
    {
        m_options.shapeBounds.yMaxPercent = yMaxPercent;
    }

    void setShapeBoundsEnabled(const bool shapeBoundsEnabled)
    {
        m_options.shapeBounds.enabled = shapeBoundsEnabled;
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

    void setScripts(const std::map<std::string, std::string>& scripts)
    {
        m_scripts = scripts;
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

bool ImageTaskPreferences::load(const std::string& filePath)
{
    return d->load(filePath);
}

bool ImageTaskPreferences::save(const std::string& filePath)
{
    return d->save(filePath);
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

void ImageTaskPreferences::setShapeBounds(const geometrize::ImageRunnerShapeBoundsOptions& shapeBounds)
{
    d->setShapeBounds(shapeBounds);
}

void ImageTaskPreferences::setShapeBounds(double xMinPercent, double yMinPercent, double xMaxPercent, double yMaxPercent)
{
    d->setShapeBounds(xMinPercent, yMinPercent, xMaxPercent, yMaxPercent);
}

void ImageTaskPreferences::setShapeBoundsXMinPercent(const double xMinPercent)
{
    d->setShapeBoundsXMinPercent(xMinPercent);
}

void ImageTaskPreferences::setShapeBoundsYMinPercent(const double yMinPercent)
{
    d->setShapeBoundsYMinPercent(yMinPercent);
}

void ImageTaskPreferences::setShapeBoundsXMaxPercent(const double xMaxPercent)
{
    d->setShapeBoundsXMaxPercent(xMaxPercent);
}

void ImageTaskPreferences::setShapeBoundsYMaxPercent(const double yMaxPercent)
{
    d->setShapeBoundsYMaxPercent(yMaxPercent);
}

void ImageTaskPreferences::setShapeBoundsEnabled(const bool shapeBoundsEnabled)
{
    d->setShapeBoundsEnabled(shapeBoundsEnabled);
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

void ImageTaskPreferences::setScripts(const std::map<std::string, std::string>& scripts)
{
    d->setScripts(scripts);
}

std::map<std::string, std::string> ImageTaskPreferences::getScripts() const
{
    return d->getScripts();
}

}

}
