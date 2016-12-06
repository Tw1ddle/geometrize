#ifndef SHAREDAPP_H
#define SHAREDAPP_H

#include <memory>

class QWidget;

namespace geometrize
{

class SharedAppImpl;

/**
 * @brief The SharedApp class is a singleton containing shared core functionality, such as implementation of common UI actions.
 */
class SharedApp
{
public:
    SharedApp();
    SharedApp& operator=(const SharedApp&) = delete;
    SharedApp(const SharedApp&) = delete;
    ~SharedApp();

    void createImageJob(QWidget* parent);
    void getImageJob(QWidget* parent) const;

private:
    std::unique_ptr<SharedAppImpl> d;
};

}

#endif // SHAREDAPP_H
