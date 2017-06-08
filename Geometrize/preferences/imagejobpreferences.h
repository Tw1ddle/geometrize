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
 * @brief The ImageJobPreferences class models the preferences associated with an image job.
 */
class ImageJobPreferences : public QObject
{
    Q_OBJECT
public:
    ImageJobPreferences();
    ImageJobPreferences(const std::string& filePath);
    ImageJobPreferences& operator=(const ImageJobPreferences& other);
    ImageJobPreferences(const ImageJobPreferences& other);
    ~ImageJobPreferences();

    /**
     * @brief load Loads the image job preferences from a file.
     * @param filePath The path to the image job preferences file.
     */
    void load(const std::string& filePath);

    /**
     * @brief save Saves the image job preferences to a file.
     * @param filePath The path to the image job preferences file.
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

    bool isScriptModeEnabled() const;
    void setScriptModeEnabled(bool enabled);
    void setScript(const std::string& scriptName, const std::string& code);
    std::map<std::string, std::string> getScripts() const;

private:
    class ImageJobPreferencesImpl;
    std::shared_ptr<ImageJobPreferencesImpl> d;
};

}

}
