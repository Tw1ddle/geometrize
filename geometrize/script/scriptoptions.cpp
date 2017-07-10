#include "scriptoptions.h"

namespace geometrize
{

namespace script
{

ScriptOptions::ScriptOptions()
{
}

ScriptOptions::ScriptOptions(const QStringList& options) : m_options{options}
{
}

ScriptOptions::ScriptOptions(const QString& options) : m_options{options.split(QRegExp("\\s"))}
{
}

QStringList ScriptOptions::getOptions() const
{
    return m_options;
}

}

}
