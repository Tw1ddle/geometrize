#pragma once

#include <string>
#include <vector>

namespace geometrize
{

namespace script
{

std::string getApplicationDirectoryPath();

void printToConsole(const std::string& str);

void debugBreak();

bool fileExists(const std::string& filePath);

bool directoryExists(const std::string& dirPath);

bool directoryContainsFile(const std::string& dirPath, const std::string& fileName);

std::string readFileAsString(const std::string& filePath);

std::vector<std::string> getFilePathsForDirectory(const std::string& dirPath);

std::vector<std::string> getScriptSearchPaths();

std::vector<std::string> getTemplateSearchPaths();

std::string getFirstFileWithExtension(const std::string& dirPath, const std::string& extension);

std::string getFirstFileWithExtensions(const std::string& dirPath, const std::vector<std::string>& extensions);

std::vector<std::string> getFilesWithExtension(const std::string& dirPath, const std::string& extension);

std::vector<std::string> getFilesWithExtensions(const std::string& dirPath, const std::vector<std::string>& extensions);

std::vector<std::string> getSupportedImageFileExtensions();

std::vector<std::string> getScriptsForPath(const std::string& dirPath);

void openJob(const std::string& url);

bool openInDefaultApplication(const std::string& path);

bool revealInDefaultApplication(const std::string& path);

void clearGlobalClipboard();

std::string getGlobalClipboardText();

void setGlobalClipboardText(const std::string& text);

}

}
