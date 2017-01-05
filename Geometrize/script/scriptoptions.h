#pragma once

#include <QStringList>

namespace geometrize
{

namespace script
{

/**
 * @brief The ScriptOptions class models a group of options that can be passed to a script.
 */
class ScriptOptions
{
public:
    ScriptOptions(const QString& options);
    ScriptOptions(const QStringList& options);
    ScriptOptions& operator=(const ScriptOptions&) = default;
    ScriptOptions(const ScriptOptions&) = default;
    ~ScriptOptions() = default;

    QStringList getOptions() const;

private:
    QStringList m_options;
};

}

}
