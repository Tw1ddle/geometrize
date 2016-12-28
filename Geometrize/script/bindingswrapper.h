#pragma once

#include <string>
#include <vector>

namespace geometrize
{

namespace script
{

std::string getApplicationDirectoryPath();

void printToConsole(const std::string& str);

bool directoryExists(const std::string& dirPath);

bool directoryContainsFile(const std::string& dirPath, const std::string& fileName);

std::string readFileAsString(const std::string& filePath);

std::vector<std::string> getFilePathsForDirectory(const std::string& dirPath);

std::vector<std::string> getScriptSearchPaths();

std::vector<std::string> getTemplateSearchPaths();

std::string getFirstFileWithExtension(const std::string& dirPath, const std::string& extension);

std::vector<std::string> getScriptsForPath(const std::string& path);

void openJob(const std::string& url);

}

}
