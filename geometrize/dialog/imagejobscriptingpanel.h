#pragma once

#include <memory>

#include <QWidget>

namespace geometrize
{

namespace dialog
{

/**
 * @brief The ImageJobScriptingPanel models the UI for the script editors for controlling image job shape setup, mutation etc.
 */
class ImageJobScriptingPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ImageJobScriptingPanel(QWidget* parent = nullptr);
    ~ImageJobScriptingPanel();

private:
    class ImageJobScriptingPanelImpl;
    std::unique_ptr<ImageJobScriptingPanelImpl> d;
};

}

}
