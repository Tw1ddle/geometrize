#include "runguard.h"

#include <QByteArray>
#include <QCryptographicHash>
#include <QSharedMemory>
#include <QString>
#include <QSystemSemaphore>

namespace geometrize
{

namespace
{

QString generateKeyHash(const QString& key, const QString& salt)
{
    QByteArray data;
    data.append(key.toUtf8());
    data.append(salt.toUtf8());
    data = QCryptographicHash::hash(data, QCryptographicHash::Sha1).toHex();
    return data;
}

}

class RunGuard::RunGuardImpl
{
public:
    RunGuardImpl(const QString& key) :
        m_key{key},
        m_memoryLockKey{generateKeyHash(key, "_memoryLockKey")},
        m_sharedMemoryKey{generateKeyHash(key, "_sharedMemoryKey")},
        m_sharedMemory{m_sharedMemoryKey},
        m_memoryLock{m_memoryLockKey, 1}
    {
        m_memoryLock.acquire();
        {
            QSharedMemory unixFix(m_sharedMemoryKey);
            unixFix.attach();
        }
        m_memoryLock.release();
    }
    ~RunGuardImpl()
    {
        release();
    }
    RunGuardImpl& operator=(const RunGuardImpl&) = delete;
    RunGuardImpl(const RunGuardImpl&) = delete;

    bool tryToRun()
    {
        if(isAnotherRunning()) {
            return false;
        }
        m_memoryLock.acquire();
        const bool result{m_sharedMemory.create(sizeof(quint64))};
        m_memoryLock.release();
        if(!result) {
            release();
            return false;
        }
        return true;
    }

    bool isAnotherRunning()
    {
        if(m_sharedMemory.isAttached()) {
            return false;
        }

        m_memoryLock.acquire();
        const bool isRunning{m_sharedMemory.attach()};
        if(isRunning) {
            m_sharedMemory.detach();
        }
        m_memoryLock.release();
        return isRunning;
    }

    void release()
    {
        m_memoryLock.acquire();
        if(m_sharedMemory.isAttached()) {
            m_sharedMemory.detach();
        }
        m_memoryLock.release();
    }

private:
    const QString m_key;
    const QString m_memoryLockKey;
    const QString m_sharedMemoryKey;

    QSharedMemory m_sharedMemory;
    QSystemSemaphore m_memoryLock;
};

RunGuard::RunGuard(const QString& key) : d{std::make_unique<RunGuard::RunGuardImpl>(key)}
{
}

RunGuard::~RunGuard()
{
}

bool RunGuard::tryToRun()
{
    return d->tryToRun();
}

bool RunGuard::isAnotherRunning()
{
    return d->isAnotherRunning();
}

}
