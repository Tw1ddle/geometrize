#include "recentitemwidget.h"
#include "ui_recentitemwidget.h"

#include <QAction>
#include <QContextMenuEvent>
#include <QMenu>

#include "common/uiactions.h"
#include "common/sharedapp.h"
#include "job/jobutil.h"
#include "util.h"

namespace geometrize
{

namespace dialog
{

RecentItemWidget::RecentItemWidget(const RecentItem& item) :
    ui(new Ui::RecentItemWidget),
    m_item{item}
{
    ui->setupUi(this);

    ui->itemDisplayName->setText(item.getDisplayName());

    const QString key{item.getKey()};
    ui->itemPath->setText(key);

    // TODO infer item type
    ui->itemTypeIcon->setPixmap(QPixmap(":/icons/folder_image.png"));

    // TODO setup context menu
}

RecentItemWidget::~RecentItemWidget()
{
    delete ui;
}

void RecentItemWidget::contextMenuEvent(QContextMenuEvent* e)
{
    QMenu itemContextMenu;

    QAction openAction(tr("Open"));
    itemContextMenu.addAction(&openAction);
    connect(&openAction, &QAction::triggered, [this]() {
        geometrize::util::openJobs({m_item.getKey()});
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

}

}
