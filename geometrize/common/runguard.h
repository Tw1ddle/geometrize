#pragma once

#include <memory>

class QString;

namespace geometrize
{

/**
 * @brief The RunGuard class is used to make an application single-instance.
 * This is like QtSingleApplication, but without a network dependency.
 * Based on code sample (which is available with no license restrictions/public domain) by Dmitry Sazonov: http://stackoverflow.com/a/28172162/1333253
 */
class RunGuard
{
public:
    RunGuard(const QString& key);
    RunGuard& operator=(const RunGuard&) = delete;
    RunGuard(const RunGuard&) = delete;
    ~RunGuard();

    bool isAnotherRunning();
    bool tryToRun();

private:
    class RunGuardImpl;
    std::unique_ptr<RunGuardImpl> d;
};

}
