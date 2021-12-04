#pragma once

#include <map>
#include <string>

namespace geometrize
{

namespace script
{

/**
 * @brief The Command class provides a convenient way for a script to tell an object about something
 * For example, this can be used to give scripts a way to control the Geometrize user interface
 */
class Command
{
public:
    Command(const std::string& command);
    virtual ~Command() = default;

    std::string getCommand() const;
    std::string getStringArg(const std::string& key) const;
    std::string getStringArg(const std::string& key, const std::string& def) const;

    Command& setStringArg(const std::string& key, const std::string& value);

private:
    const std::string m_command;
    std::map<std::string, std::string> m_stringArgs;
};

}

}
