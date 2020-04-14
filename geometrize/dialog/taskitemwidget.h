#pragma once

#include <functional>
#include <memory>

#include <QString>
#include <QWidget>

class QContextMenuEvent;
class QEvent;
class QMouseEvent;

namespace geometrize
{

namespace dialog
{

/**
 * @brief The TaskItemWidget class is a button that opens/shows a task when pressed (such as a recently opened image, when used on the recently-opened items list).
 */
class TaskItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TaskItemWidget(const QString& taskItemId, const QString& taskItemDisplayName, const std::function<void(const QString&)>& openItemCb, const std::function<void(const QString&)>& removeItemCb);
    virtual ~TaskItemWidget();

protected:
    void changeEvent(QEvent*) override;

private:
    virtual void contextMenuEvent(QContextMenuEvent* e) override;
    virtual void mouseReleaseEvent(QMouseEvent* e) override;

    class TaskItemWidgetImpl;
    std::unique_ptr<TaskItemWidget::TaskItemWidgetImpl> d;
};

}

}
