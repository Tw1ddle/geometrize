#pragma once

#include <set>
#include <string>

#include "script/command.h"

namespace geometrize
{

namespace script
{

/**
 * @brief The CommandHandler class provides an interface that all objects that can handle/process script commands
 * implement. This is used do things like provide a way for scripts to send commands to the Geometrize user interface
 */
class CommandHandler
{
public:
    CommandHandler();
    virtual ~CommandHandler();

    virtual void handleCommand(const Command& command) = 0;
    virtual std::string getCommandHandlerName() const = 0;
    virtual void setCommandHandlerName(const std::string& name) = 0;

    static std::set<CommandHandler*> allCommandHandlers;
};

}

}
