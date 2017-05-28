#pragma once

#include <memory>

#include <QGroupBox>

class QLayout;

namespace geometrize
{

namespace dialog
{

/**
 * @brief The CollapsiblePanel class models the UI for a collapsible container groupbox.
 */
class CollapsiblePanel : public QGroupBox
{
    Q_OBJECT

public:
    CollapsiblePanel(QWidget* parent = nullptr);

    void setup();

private:
    class CollapsiblePanelImpl;
    std::unique_ptr<CollapsiblePanelImpl> d;
};

}

}
