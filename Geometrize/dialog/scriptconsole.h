#pragma once

#include <memory>
#include <string>
#include <vector>

#include <QWidget>

namespace chaiscript
{
class ChaiScript;
}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The ScriptConsole class models a command line and output window.
 */
class ScriptConsole : public QWidget
{
    Q_OBJECT

public:
    explicit ScriptConsole(QWidget* parent = 0);
    ~ScriptConsole();

    static const std::string launchConsoleHistoryFilename; ///< The name of the console history settings file used by the console embedded in the launch window.

    /**
     * @brief getConsoleHistoryFilenameForJob Gets the name of the settings file used by the console of the given job url. Should be unique for each job.
     * @param jobUrl The job url, such as the file path or URL used for the file.
     * @return The name of the console history settings file for the given job.
     */
    static std::string getConsoleHistoryFilenameForJob(const std::string& jobUrl);

    void setEngine(chaiscript::ChaiScript* engine);
    std::vector<std::string> getHistory() const;
    void setHistory(const std::vector<std::string>& history);

private:
    class ScriptConsoleImpl;
    std::unique_ptr<ScriptConsoleImpl> d;
};

}

}
