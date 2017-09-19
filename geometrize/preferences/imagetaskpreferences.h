#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <string>

#include <QObject>

#include "geometrize/runner/imagerunneroptions.h"

namespace geometrize
{

namespace preferences
{

/**
 * @brief The ImageTaskPreferences class models the preferences associated with an image task.
 */
class ImageTaskPreferences : public QObject
{
    Q_OBJECT
public:
    ImageTaskPreferences();
    ImageTaskPreferences(const std::string& filePath);
    ImageTaskPreferences& operator=(const ImageTaskPreferences& other);
    ImageTaskPreferences(const ImageTaskPreferences& other);
    ~ImageTaskPreferences();

    /**
     * @brief load Loads the image task preferences from a file.
     * @param filePath The path to the image task preferences file.
     */
    void load(const std::string& filePath);

    /**
     * @brief save Saves the image task preferences to a file.
     * @param filePath The path to the image task preferences file.
     */
    void save(const std::string& filePath);

    /**
     * @brief getImageRunnerOptions Gets a copy of the image runner options.
     * @return The image runner options.
     */
    geometrize::ImageRunnerOptions getImageRunnerOptions() const;

    void enableShapeTypes(geometrize::ShapeTypes shapes);
    void disableShapeTypes(geometrize::ShapeTypes shapes);
    void setShapeTypes(geometrize::ShapeTypes shapes);
    void setShapeAlpha(std::uint8_t alpha);
    void setCandidateShapeCount(std::uint32_t shapeCount);
    void setMaxShapeMutations(std::uint32_t maxMutations);
    void setSeed(std::uint32_t seed);
    void setMaxThreads(std::uint32_t maxThreads);

    bool isScriptModeEnabled() const;
    void setScriptModeEnabled(bool enabled);
    void setScript(const std::string& scriptName, const std::string& code);
    void setScripts(const std::map<std::string, std::string>& scripts);
    std::map<std::string, std::string> getScripts() const;

private:
    class ImageTaskPreferencesImpl;
    std::shared_ptr<ImageTaskPreferencesImpl> d;
};

}

}
