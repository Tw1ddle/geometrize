#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <string>

#include "geometrize/runner/imagerunneroptions.h"

namespace geometrize
{

namespace preferences
{

/**
 * @brief getImageTaskPreferencesAutoSavePath Gets the absolute path to where the image task preferences config file should be read/written.
 * @param hashOfFirstTargetImage A string containing a hash of the first target image the task was set up to use.
 * NOTE - this hash changes based on the target image, so modifying the "resize image" settings in the application will cause this to change.
 * @return The name of the autosave task settings file, including the file extension
 * @return The absolute path the image task auto save path.
 */
std::string getImageTaskPreferencesAutoSavePath(const std::string& hashOfFirstTargetImage);

/**
 * @brief The ImageTaskPreferences class models the preferences associated with an image task.
 */
class ImageTaskPreferences
{
public:
    ImageTaskPreferences();
    ImageTaskPreferences(const std::string& filePath);
    ImageTaskPreferences& operator=(const ImageTaskPreferences& other);
    ImageTaskPreferences(const ImageTaskPreferences& other);
    virtual ~ImageTaskPreferences();

    /**
     * @brief load Loads the image task preferences from a file.
     * @param filePath The path to the image task preferences file.
     * @param return True if the image task preferences were loaded successfully, else false.
     */
    bool load(const std::string& filePath);

    /**
     * @brief save Saves the image task preferences to a file.
     * @param filePath The path to the image task preferences file.
     * @param return True if the image task preferences were saved successfully, else false.
     */
    bool save(const std::string& filePath);

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
    void setShapeBounds(const geometrize::ImageRunnerShapeBoundsOptions& shapeBounds);
    void setShapeBoundsXMin(int xMin);
    void setShapeBoundsYMin(int yMin);
    void setShapeBoundsXMax(int xMax);
    void setShapeBoundsYMax(int yMax);
    void setShapeBoundsEnabled(bool shapeBoundsEnabled);

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
