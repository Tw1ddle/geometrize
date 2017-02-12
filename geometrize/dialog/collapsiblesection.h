#pragma once

#include <memory>

#include <QGroupBox>

class QLayout;

namespace geometrize
{

namespace dialog
{

/**
 * @brief The CollapsibleSection class models the UI for a collapsible container groupbox.
 */
class CollapsibleSection : public QGroupBox
{
public:
    CollapsibleSection(QWidget* parent = nullptr);

    void setContentLayout(QLayout& contentLayout);

private:
    class CollapsibleSectionImpl;
    std::unique_ptr<CollapsibleSectionImpl> d;
};

}

}
