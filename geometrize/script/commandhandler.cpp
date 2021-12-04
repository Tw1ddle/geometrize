#include "commandhandler.h"

#include <set>

namespace geometrize
{

namespace script
{

std::set<CommandHandler*> CommandHandler::allCommandHandlers;

CommandHandler::CommandHandler()
{
    allCommandHandlers.insert(this);
}

CommandHandler::~CommandHandler()
{
    allCommandHandlers.erase(this);
}

}

}
