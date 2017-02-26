#include "recentitemwidget.h"
#include "ui_recentitemwidget.h"

#include <QAction>
#include <QContextMenuEvent>
#include <QFuture>
#include <QFutureWatcher>
#include <QImage>
#include <QMenu>
#include <QtConcurrent/QtConcurrentRun>

#include "common/uiactions.h"
#include "common/sharedapp.h"
#include "job/jobutil.h"
#include "recentitem.h"
#include "recentitems.h"
#include "util.h"

namespace geometrize
{

namespace dialog
{

class RecentItemWidget::RecentItemWidgetImpl
{
public:
    RecentItemWidgetImpl(RecentItemWidget* pQ, const RecentItem& item) : q{pQ}, ui{std::make_unique<Ui::RecentItemWidget>()}, m_item{item}
    {
        ui->setupUi(q);

        ui->itemDisplayName->setText(item.getDisplayName());

        const QString key{item.getKey()};
        ui->itemPath->setText(key);

        q->connect(&m_thumbnailLoaderWatcher, &QFutureWatcher<QImage>::finished, [this]() {
            const QImage thumbnail{m_thumbnailLoaderWatcher.future().result()};
            ui->thumbnailIcon->setPixmap(QPixmap::fromImage(thumbnail));
        });

        const QString itemPath{item.getKey()};
        const RecentItem::Type type{RecentItem::getTypeForKey(itemPath)};

        QFuture<QImage> thumbnailFuture{QtConcurrent::run(this, &RecentItemWidgetImpl::setupThumbnail, itemPath, type)};
        m_thumbnailLoaderWatcher.setFuture(thumbnailFuture);
    }

    ~RecentItemWidgetImpl()
    {
    }

    void onContextMenuEvent(QContextMenuEvent* e)
    {
        QMenu itemContextMenu;

        QAction openAction(tr("Open"));
        itemContextMenu.addAction(&openAction);
        connect(&openAction, &QAction::triggered, [this]() {
            geometrize::util::openJobs({m_item.getKey()}, false);
        });

        QAction openInDefaultViewer(tr("Open in viewer"));
        itemContextMenu.addAction(&openInDefaultViewer);
        connect(&openInDefaultViewer, &QAction::triggered, [this]() {
            geometrize::util::openInDefaultApplication(m_item.getKey().toStdString());
        });

        QAction revealInDefaultExplorer(tr("Reveal in explorer"));
        itemContextMenu.addAction(&revealInDefaultExplorer);
        connect(&revealInDefaultExplorer, &QAction::triggered, [this]() {
            geometrize::util::revealInDefaultApplication(m_item.getKey().toStdString());
        });

        QAction copyToClipboard(tr("Copy path to clipboard"));
        itemContextMenu.addAction(&copyToClipboard);
        connect(&copyToClipboard, &QAction::triggered, [this]() {
            geometrize::util::setGlobalClipboardText(m_item.getKey().toStdString());
        });

        QAction removalAction(tr("Remove from list"));
        itemContextMenu.addAction(&removalAction);
        connect(&removalAction, &QAction::triggered, [this]() {
            geometrize::common::app::SharedApp::get().getRecentFiles().remove(m_item.getKey());
        });

        itemContextMenu.exec(e->globalPos());
    }

    void onMouseReleaseEvent(QMouseEvent* e)
    {
        if(e->button() != Qt::LeftButton) {
            return;
        }

        geometrize::util::openJobs({m_item.getKey()}, false);
    }

    void onKeyPressEvent(QKeyEvent* e)
    {
        if(e->key() != Qt::Key_Return || e->key() != Qt::Key_Enter) {
            return;
        }

        geometrize::util::openJobs({m_item.getKey()}, false);
    }

private:
    QImage setupThumbnail(const QString& itemPath, const RecentItem::Type type)
    {
        switch(type) {
            case RecentItem::Type::LOCAL_IMAGE:
            {
                const QImage thumbnail(itemPath);
                if(!thumbnail.isNull()) {
                    const QSize size{100, 100};
                    return thumbnail.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
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

    RecentItemWidget* q;
    std::unique_ptr<Ui::RecentItemWidget> ui;
    RecentItem m_item;
    QFutureWatcher<QImage> m_thumbnailLoaderWatcher;
};

RecentItemWidget::RecentItemWidget(const RecentItem& item) : d{std::make_unique<RecentItemWidget::RecentItemWidgetImpl>(this, item)}
{
}

RecentItemWidget::~RecentItemWidget()
{
}

void RecentItemWidget::contextMenuEvent(QContextMenuEvent* e)
{
    d->onContextMenuEvent(e);

    QWidget::contextMenuEvent(e);
}

void RecentItemWidget::mouseReleaseEvent(QMouseEvent* e)
{
    d->onMouseReleaseEvent(e);

    QWidget::mouseReleaseEvent(e);
}

}

}
