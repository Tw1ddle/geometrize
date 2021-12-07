#include "taskitemwidget.h"
#include "ui_taskitemwidget.h"

#include <functional>

#include <QAction>
#include <QContextMenuEvent>
#include <QEvent>
#include <QFile>
#include <QFuture>
#include <QFutureWatcher>
#include <QImage>
#include <QMenu>
#include <QMouseEvent>
#include <QString>
#include <QtConcurrent/QtConcurrentRun>

#include "common/uiactions.h"
#include "common/util.h"
#include "recents/recentitems.h"
#include "task/taskutil.h"

namespace geometrize
{

namespace dialog
{

class TaskItemWidget::TaskItemWidgetImpl
{
public:
    TaskItemWidgetImpl(TaskItemWidget* pQ, const QString& taskItemId, const QString& taskItemDisplayName, const std::function<void(const QString&)>& openItemCb, const std::function<void(const QString&)>& removeItemCb) :
        q{pQ}, ui{std::make_unique<Ui::TaskItemWidget>()}, m_taskItemId{taskItemId}, m_taskItemDisplayName{taskItemDisplayName}, m_openItemCb{openItemCb}, m_removeItemCb{removeItemCb}
    {
        ui->setupUi(q);

        ui->itemDisplayName->setText(m_taskItemDisplayName);

        ui->itemPath->setText(m_taskItemId);

        q->connect(&m_thumbnailLoaderWatcher, &QFutureWatcher<QImage>::finished, [this]() {
            const QImage thumbnail{m_thumbnailLoaderWatcher.future().result()};
            ui->thumbnailIcon->setPixmap(QPixmap::fromImage(thumbnail));
        });

        const QString itemPath{m_taskItemId};
        const RecentItem::Type type{RecentItem::getTypeForKey(itemPath)};

        QFuture<QImage> thumbnailFuture = QtConcurrent::run([this, itemPath, type]() {
            return setupThumbnail(itemPath, type);
        });
        m_thumbnailLoaderWatcher.setFuture(thumbnailFuture);
    }

    ~TaskItemWidgetImpl()
    {
    }

    void onContextMenuEvent(QContextMenuEvent* e)
    {
        QMenu itemContextMenu;

        QAction openAction(tr("Open", "Text on a menu item the user presses to open a file/image"));
        itemContextMenu.addAction(&openAction);
        connect(&openAction, &QAction::triggered, [this]() {
            m_openItemCb(m_taskItemId);
        });

        QAction openInDefaultViewer(tr("Open in viewer", "Text on a menu item the user presses to open an image/piece of media in a viewer"));
        itemContextMenu.addAction(&openInDefaultViewer);
        connect(&openInDefaultViewer, &QAction::triggered, [this]() {
            geometrize::util::openInDefaultApplication(m_taskItemId.toStdString());
        });

        QAction revealInDefaultExplorer(tr("Reveal in file explorer", "Text on a menu item the user presses to open an image/piece of media in a file viewer/explorer"));
        itemContextMenu.addAction(&revealInDefaultExplorer);
        connect(&revealInDefaultExplorer, &QAction::triggered, [this]() {
            geometrize::util::revealInDefaultApplication(m_taskItemId.toStdString());
        });

        QAction copyToClipboard(tr("Copy file path to clipboard", "Text on a menu item the user selects to copy a file path to the copy-paste clipboard"));
        itemContextMenu.addAction(&copyToClipboard);
        connect(&copyToClipboard, &QAction::triggered, [this]() {
            geometrize::util::setGlobalClipboardText(m_taskItemId.toStdString());
        });

        QAction removalAction(tr("Remove from list", "Text on a menu item the user presses to remove an item from a list of items"));
        itemContextMenu.addAction(&removalAction);
        connect(&removalAction, &QAction::triggered, [this]() {
            m_removeItemCb(m_taskItemId);
        });

        itemContextMenu.exec(e->globalPos());
    }

    void onMouseReleaseEvent(QMouseEvent* e)
    {
        if(e->button() != Qt::LeftButton) {
            return;
        }

        m_openItemCb(m_taskItemId);
    }

    void onKeyPressEvent(QKeyEvent* e)
    {
        if(e->key() != Qt::Key_Return && e->key() != Qt::Key_Enter) {
            return;
        }

        m_openItemCb(m_taskItemId);
    }

    void onLanguageChange()
    {
        ui->retranslateUi(q);
        populateUi();
    }

private:
    void populateUi()
    {

    }

    QImage setupThumbnail(const QString& itemPath, const RecentItem::Type type)
    {
        switch(type) {
            case RecentItem::Type::LOCAL_IMAGE:
            {
                QImage thumbnail;
                if(QFile(itemPath).exists()) {
                    thumbnail = QImage(itemPath);
                } else {
                    thumbnail = QImage(QUrl(itemPath).toLocalFile());
                }
                if(!thumbnail.isNull()) {
                    thumbnail = thumbnail.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    return thumbnail;
                }
                thumbnail = QImage(":/icons/image.png");
                if(!thumbnail.isNull()) {
                    return thumbnail;
                }
                break;
            }
            case RecentItem::Type::LOCAL_CHAISCRIPT:
            {
                const QImage thumbnail(":/icons/script_go.png");
                if(!thumbnail.isNull()) {
                    return thumbnail;
                }
                break;
            }
            case RecentItem::Type::REMOTE_RESOURCE:
            {
                const QImage thumbnail(":/icons/world_link.png");
                if(!thumbnail.isNull()) {
                    return thumbnail;
                }
                break;
            }
            case RecentItem::Type::UNKNOWN:
            {
                break;
            }
        }

        return QImage(":/icons/error.png");
    }

    TaskItemWidget* q;
    std::unique_ptr<Ui::TaskItemWidget> ui;
    const QString m_taskItemId;
    const QString m_taskItemDisplayName;
    const std::function<void(const QString&)> m_openItemCb;
    const std::function<void(const QString&)> m_removeItemCb;
    QFutureWatcher<QImage> m_thumbnailLoaderWatcher;
};

TaskItemWidget::TaskItemWidget(const QString& taskItemId, const QString& taskItemDisplayName, const std::function<void(const QString&)>& openItemCb, const std::function<void(const QString&)>& removeItemCb) :
    d{std::make_unique<TaskItemWidget::TaskItemWidgetImpl>(this, taskItemId, taskItemDisplayName, openItemCb, removeItemCb)}
{
}

TaskItemWidget::~TaskItemWidget()
{
}

void TaskItemWidget::contextMenuEvent(QContextMenuEvent* e)
{
    d->onContextMenuEvent(e);

    QWidget::contextMenuEvent(e);
}

void TaskItemWidget::mouseReleaseEvent(QMouseEvent* e)
{
    d->onMouseReleaseEvent(e);

    QWidget::mouseReleaseEvent(e);
}

void TaskItemWidget::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        d->onLanguageChange();
    }
    QWidget::changeEvent(event);
}

}

}
