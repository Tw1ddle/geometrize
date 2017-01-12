#pragma once

#include <QWidget>

#include "recentitems.h"

namespace Ui
{
class RecentItemWidget;
}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The RecentItemWidget class is a button that opens a recent job when pressed, like a recently opened image.
 */
class RecentItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RecentItemWidget(const RecentItem& item);
    ~RecentItemWidget();

private:
    virtual void contextMenuEvent(QContextMenuEvent* e) override;

    Ui::RecentItemWidget* ui;
    RecentItem m_item;
};

}

}
