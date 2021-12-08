#include "bindingswrapper.h"

#include <cassert>
#include <string>
#include <vector>

#include <QString>
#include <QStringList>
#include <QObject>
#include <QWidget>

#include "common/formatsupport.h"
#include "common/searchpaths.h"
#include "common/util.h"
#include "dialog/launchwindow.h"
#include "dialog/imagetaskwindow.h"
#include "exporter/gifexporter.h"
#include "geometrize/bitmap/bitmap.h"
#include "localization/localization.h"
#include "scene/customtabletevent.h"
#include "script/command.h"
#include "script/commandhandler.h"
#include "task/taskutil.h"
#include "task/imagetask.h"

namespace geometrize
{

namespace script
{

namespace bindings
{

geometrize::dialog::ImageTaskWindow* createImageTaskWindow()
{
    return new dialog::ImageTaskWindow();
}

geometrize::dialog::LaunchWindow* createLaunchWindow()
{
    return new dialog::LaunchWindow();
}

geometrize::task::ImageTask* createImageTask(geometrize::Bitmap& bitmap)
{
    return new task::ImageTask(bitmap);
}

std::vector<geometrize::dialog::LaunchWindow*> getLaunchWindows()
{
    return geometrize::dialog::LaunchWindow::getTopLevelLaunchWindows();
}

std::vector<geometrize::dialog::ImageTaskWindow*> getImageTaskWindows()
{
    return geometrize::dialog::ImageTaskWindow::getExistingImageTaskWindows();
}

std::string getApplicationDirectoryPath()
{
    return geometrize::searchpaths::getApplicationDirectoryPath();
}

void printToConsole(const std::string& str)
{
    geometrize::util::printToConsole(str);
}

void printToAllScriptConsoleWidgets(const std::string& str)
{
    geometrize::util::printToAllScriptConsoleWidgets(str);
}

void messageBox(const std::string& str)
{
    geometrize::util::messageBox(str);
}

void debugBreak()
{
    geometrize::util::debugBreak();
}

void processApplicationEvents()
{
    geometrize::util::processApplicationEvents();
}

bool fileExists(const std::string& filePath)
{
    return geometrize::util::fileExists(filePath);
}

bool directoryExists(const std::string& dirPath)
{
    return geometrize::util::directoryExists(dirPath);
}

bool directoriesExist(const std::vector<std::string>& dirPaths)
{
    return geometrize::util::directoriesExist(dirPaths);
}

bool directoryContainsFile(const std::string& dirPath, const std::string& fileName)
{
    return geometrize::util::directoryContainsFile(dirPath, fileName);
}

bool createDirectory(const std::string& dirPath)
{
    return geometrize::util::createDirectory(dirPath);
}

std::string readFileAsString(const std::string& filePath)
{
    return geometrize::util::readFileAsString(filePath);
}

std::vector<std::string> getFilePathsForDirectory(const std::string& dirPath)
{
    return geometrize::util::getFilePathsForDirectory(dirPath);
}

std::vector<std::string> getScriptSearchPaths()
{
    return geometrize::searchpaths::getScriptSearchPaths();
}

std::vector<std::string> getTemplateSearchPaths()
{
    return geometrize::searchpaths::getTemplateSearchPaths();
}

std::string getFirstFileWithExtension(const std::string& dirPath, const std::string& extension)
{
    return geometrize::util::getFirstFileWithExtension(dirPath, extension);
}

std::string getFirstFileWithExtensions(const std::string& dirPath, const std::vector<std::string>& extensions)
{
    return geometrize::util::getFirstFileWithExtensions(dirPath, extensions);
}

std::vector<std::string> getFilesWithExtensions(const std::string& /*dirPath*/, const std::vector<std::string>& /*extensions*/)
{
    std::vector<std::string> files;

    //for(const std::string& extension : extensions) {
        // TODO
    //}

    return files;
}

std::vector<std::string> getFilesWithExtension(const std::string& dirPath, const std::string& extension)
{
    return getFilesWithExtensions(dirPath, { extension });
}

std::vector<std::string> getSupportedImageFileExtensions()
{
    return geometrize::format::getReadableImageFileExtensions(false);
}

std::vector<std::string> getScriptsForPath(const std::string& dirPath)
{
    return geometrize::util::getScriptsForPath(dirPath);
}

std::vector<std::string> getScriptsForPaths(const std::vector<std::string>& dirPaths)
{
    return geometrize::util::getScriptsForPaths(dirPaths);
}

std::string getDirectoryForFilePath(const std::string& filePath)
{
    return geometrize::util::getDirectoryForFilePath(filePath);
}

std::string getFileNameForFilePath(const std::string& filePath)
{
    return geometrize::util::getFileNameForFilePath(filePath);
}

void openTask(const std::string& url, const bool addToRecents)
{
    // Horrible workaround because QUrl doesn't handle Qt resource file prefixes well
    // See: https://forum.qt.io/topic/1494/universal-solution-for-resource-prefix
    // Note this breaks QUrl validation too (QUrl isValid chokes when given this modified url)
    QString qUrl{QString::fromStdString(url)};
    const QString strToReplace{":/"};
    if(qUrl.startsWith(strToReplace)) {
        qUrl.replace(0, strToReplace.size(), "qrc:///");
    }

    geometrize::util::openTasks(QStringList(QString::fromStdString(url)), addToRecents);
}

bool openInDefaultApplication(const std::string& path)
{
    return geometrize::util::openInDefaultApplication(path);
}

bool revealInDefaultApplication(const std::string& path)
{
    return geometrize::util::revealInDefaultApplication(path);
}

void clearGlobalClipboard()
{
    geometrize::util::clearGlobalClipboard();
}

std::string getGlobalClipboardText()
{
    return geometrize::util::getGlobalClipboardText();
}

void setGlobalClipboardText(const std::string& text)
{
    geometrize::util::setGlobalClipboardText(text);
}

bool stringBeginsWith(const std::string& str, const std::string& prefix)
{
    return geometrize::util::stringBeginsWith(str, prefix);
}

bool stringEndsWith(const std::string& str, const std::string& suffix)
{
    return geometrize::util::stringEndsWith(str, suffix);
}

std::string getApplicationDirectoryLocation()
{
    return geometrize::util::getApplicationDirectoryLocation();
}

std::string getAppDataLocation()
{
    return geometrize::util::getAppDataLocation();
}

std::string getHomeDirectoryLocation()
{
    return geometrize::util::getHomeDirectoryLocation();
}

std::string getDesktopDirectoryLocation()
{
    return geometrize::util::getDesktopDirectoryLocation();
}

bool writeStringToFile(const std::string& str, const std::string& path)
{
    return geometrize::util::writeStringToFile(str, path);
}

std::string percentEncode(const std::string& str)
{
    return geometrize::util::percentEncode(str);
}

int randomIntInRange(const int lower, const int upper)
{
    return randomInRange<int, int, int>(lower, upper);
}

float randomFloatInRange(const float lower, const float upper)
{
    return randomInRange<float, float, float>(lower, upper);
}

int clampInt(const int value, const int lower, const int upper)
{
    return clamp<int, int, int>(value, lower, upper);
}

float clampFloat(const float value, const float lower, const float upper)
{
    return clamp<float, float, float>(value, lower, upper);
}

std::vector<std::string> split(const std::string& s, const char delimiter)
{
    return geometrize::util::split(s, delimiter);
}

std::string getOperatingSystemProductType()
{
    return geometrize::util::getOperatingSystemProductType();
}

int getCursorX()
{
    return geometrize::util::getCursorX();
}

int getCursorY()
{
    return geometrize::util::getCursorY();
}

void setCursorPos(const int x, const int y)
{
    geometrize::util::setCursorPos(x, y);
}

void setTranslatorsForLocale(const std::string& locale)
{
    geometrize::setTranslatorsForLocale(QString::fromStdString(locale));
}

std::vector<std::string> getSupportedLocaleCodes()
{
    QStringList codes = geometrize::getSupportedLocaleCodes();
    std::vector<std::string> v;
    for(const QString& s : codes) {
        v.emplace_back(s.toStdString());
    }
    return v;
}

void setLocaleAndUserInterfaceLanguage(const std::string& locale)
{
    geometrize::setLocaleAndUserInterfaceLanguage(QString::fromStdString(locale));
}

bool exportGIF(const std::vector<geometrize::ShapeResult>& data,
        const std::uint32_t inputWidth,
        const std::uint32_t inputHeight,
        const std::uint32_t outputWidth,
        const std::uint32_t outputHeight,
        const std::size_t frameSkip,
        const std::string& filePath)
{
    return geometrize::exporter::exportGIF(data, inputWidth, inputHeight, outputWidth, outputHeight, [frameSkip](std::size_t frameIndex) {
        return frameSkip == 0U ? false : (frameIndex % frameSkip == 0);
    }, filePath);
}

bool saveWidgetScreenshot(const std::string& path, QWidget* widget)
{
    return geometrize::util::saveWidgetScreenshot(path, widget);
}

std::string customTabletEventDataToString(const geometrize::scene::TabletEventData& data)
{
    return geometrize::scene::tabletEventDataToString(data).toStdString();
}

std::vector<std::string> getAllNamedWidgets()
{
    return geometrize::util::getAllNamedWidgets();
}

std::vector<std::string> getAllNamedGeometrizeWidgets()
{
    return geometrize::util::getAllNamedGeometrizeWidgets();
}

QWidget* getWidgetByName(const std::string& widgetName)
{
    return geometrize::util::getWidgetByName(widgetName);
}

std::vector<std::string> getAllNamedCommandHandlers()
{
    return geometrize::util::getAllNamedCommandHandlers();
}

CommandHandler* getCommandHandlerByName(const std::string& name)
{
    return geometrize::util::getCommandHandlerByName(name);
}

void sendCommandString(const std::string& target, const std::string& command)
{
    geometrize::util::sendCommand(target, command);
}

void sendCommandObject(const std::string& target, Command& command)
{
    geometrize::util::sendCommand(target, command);
}

void broadcastCommandString(const std::string& command)
{
    geometrize::util::broadcastCommand(command);
}

void broadcastCommandObject(Command& command)
{
    geometrize::util::broadcastCommand(command);
}

std::string getFormattedTimestamp(const std::string& formatString)
{
    return geometrize::util::getFormattedTimestamp(formatString);
}

std::string getFilenameTimestamp()
{
    return geometrize::util::getFilenameTimestamp();
}

std::string getUuidString()
{
    return geometrize::util::getUuidString();
}

void setWidgetSize(QWidget* widget, int width, int height)
{
    geometrize::util::setWidgetSize(widget, width, height);
}

void setWidgetPosition(QWidget* widget, int x, int y)
{
    geometrize::util::setWidgetPosition(widget, x, y);
}

void arrangeWidgetsInGrid(const std::vector<QWidget*>& widgets, int centerX, int centerY, int xDim, int yDim, int resizeX, int resizeY)
{
    geometrize::util::arrangeWidgetsInGrid(widgets, centerX, centerY, xDim, yDim, resizeX, resizeY);
}

void arrangeImageTaskWidgetsInGrid(int centerX, int centerY, int xDim, int yDim, int resizeX, int resizeY)
{
    geometrize::util::arrangeImageTaskWidgetsInGrid(centerX, centerY, xDim, yDim, resizeX, resizeY);
}

void arrangeWidgetsInGridOnMonitor(const std::vector<QWidget*>& widgets, int monitorId, int resizeX, int resizeY)
{
    geometrize::util::arrangeWidgetsInGridOnMonitor(widgets, monitorId, resizeX, resizeY);
}

void arrangeImageTaskWidgetsInGridOnMonitor(int monitorId, int resizeX, int resizeY)
{
    geometrize::util::arrangeImageTaskWidgetsInGridOnMonitor(monitorId, resizeX, resizeY);
}

void arrangeWidgetsInGridOnPrimaryMonitor(const std::vector<QWidget*>& widgets, int resizeX, int resizeY)
{
    geometrize::util::arrangeWidgetsInGridOnPrimaryMonitor(widgets, resizeX, resizeY);
}

void arrangeImageTaskWidgetsInGridOnPrimaryMonitor(int resizeX, int resizeY)
{
    geometrize::util::arrangeImageTaskWidgetsInGridOnPrimaryMonitor(resizeX, resizeY);
}

void fitWidgetsInGridOnMonitor(const std::vector<QWidget*>& widgets, int screenIdx)
{
    geometrize::util::fitWidgetsInGridOnMonitor(widgets, screenIdx);
}

void fitImageTaskWidgetsInGridOnMonitor(int screenIdx)
{
    geometrize::util::fitImageTaskWidgetsInGridOnMonitor(screenIdx);
}

void fitWidgetsInGridOnPrimaryMonitor(const std::vector<QWidget*>& widgets)
{
    geometrize::util::fitWidgetsInGridOnPrimaryMonitor(widgets);
}

void fitImageTaskWidgetsInGridOnPrimaryMonitor()
{
    geometrize::util::fitImageTaskWidgetsInGridOnPrimaryMonitor();
}

}

}

}
