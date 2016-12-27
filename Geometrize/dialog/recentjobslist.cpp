#include "recentjobslist.h"

#include "assert.h"

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QContextMenuEvent>
#include <QPoint>

#include "recentitems.h"

namespace geometrize
{

namespace dialog
{

RecentJobsList::RecentJobsList(QWidget *parent) : QListWidget(parent), m_recents{nullptr}
{
}

void RecentJobsList::setRecentItems(RecentItems* recents)
{
    // TODO disconnect

    m_recents = recents;

    // TODO sort
    const auto items{recents->getItems()};
    for(auto& item : items) {
        addItem(item.second);
    }

    if(m_recents != nullptr) {
        connect(m_recents, &RecentItems::signal_added, [this](const QString& item, const bool preexisting) {
            qDebug() << "ADDED" << " " << item;
            if(preexisting) {
                const QList<QListWidgetItem*> items{findItems(item, Qt::MatchExactly)};
                for(auto* item : items) {
                    removeItemWidget(item);
                }
            } else {
                addItem(item);
            }
        });
        connect(m_recents, &RecentItems::signal_cleared, [this]() {
            qDebug() << "CLEARED";
            clear();
        });
        connect(m_recents, &RecentItems::signal_removed, [this](const QString& item) {
            qDebug() << "REMOVED" << " " << item;
            const QList<QListWidgetItem*> items{findItems(item, Qt::MatchExactly)};
            for(auto* item : items) {
                removeItemWidget(item);
            }
        });
    }
}

RecentItems* RecentJobsList::getRecentItems()
{
    return m_recents;
}

void RecentJobsList::contextMenuEvent(QContextMenuEvent* e)
{
    if(e->reason() != QContextMenuEvent::Mouse) {
        return;
    }
    emit signal_contextMenuRequested(itemAt(e->pos()), mapToGlobal(e->pos()));
}

void RecentJobsList::keyPressEvent(QKeyEvent* e)
{
    QListWidget::keyPressEvent(e);

    if(e->matches(QKeySequence::Copy)) {
        const QList<QListWidgetItem*> items{selectedItems()};
        QStringList strings;
        for(const QListWidgetItem* item : items) {
            strings.push_back(item->text());
        }
        QApplication::clipboard()->setText(strings.join("\n"));
    } else if(e->key() == Qt::Key_Delete || e->key() == Qt::Key_Backspace) {
        qDebug() << "TODO remove item";
    }
}

}

}
