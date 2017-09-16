#pragma once

#include <memory>

#include <QWidget>

class QEvent;

namespace Ui
{
class RecentItemWidget;
}

namespace geometrize
{
class RecentItem;
}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The RecentItemWidget class is a button that opens a recent task when pressed, like a recently opened image.
 */
class RecentItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RecentItemWidget(const RecentItem& item);
    ~RecentItemWidget();

protected:
    void changeEvent(QEvent*) override;

private:
    virtual void contextMenuEvent(QContextMenuEvent* e) override;
    virtual void mouseReleaseEvent(QMouseEvent* e) override;

    class RecentItemWidgetImpl;
    std::unique_ptr<RecentItemWidget::RecentItemWidgetImpl> d;
};

}

}
