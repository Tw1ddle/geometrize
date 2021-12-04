#include "script/command.h"

#include <string>

namespace geometrize
{

namespace script
{

Command::Command(const std::string& command) : m_command{command}
{
}

std::string Command::getCommand() const
{
    return m_command;
}

std::string Command::getStringArg(const std::string& key) const
{
    const auto it = m_stringArgs.find(key);
    return it == m_stringArgs.end() ? "" : it->second;
}

std::string Command::getStringArg(const std::string& key, const std::string& def) const
{
    const auto it = m_stringArgs.find(key);
    return it == m_stringArgs.end() ? def : it->second;
}

Command& Command::setStringArg(const std::string& key, const std::string& value)
{
    m_stringArgs[key] = value;
    return *this;
}

}

}
