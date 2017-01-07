#include "recentjobslist.h"

#include "assert.h"

#include <QApplication>
#include <QClipboard>
#include <QContextMenuEvent>

#include "recentitemwidget.h"
#include "recentitem.h"
#include "recentitems.h"

namespace geometrize
{

namespace dialog
{

class RecentJobsList::RecentJobsListImpl
{
public:
    RecentJobsListImpl(RecentJobsList* pQ) : q{pQ}
    {
        q->setSizeAdjustPolicy(QListWidget::AdjustToContents);
    }

    RecentJobsListImpl operator=(const RecentJobsListImpl&) = delete;
    RecentJobsListImpl(const RecentJobsListImpl&) = delete;
    ~RecentJobsListImpl() = default;

    RecentItems* getRecentItems()
    {
        return m_recents;
    }

    void setRecentItems(RecentItems* recents)
    {
        m_recents = recents;
        q->clear();
        setupConnections();
        loadExistingItems();
    }

private:
    void loadExistingItems()
    {
        if(m_recents == nullptr) {
            return;
        }

        // TODO sort
        const auto items{m_recents->getItems()};
        for(const auto& item : items) {
            addItem(item);
        }
    }

    void setupConnections()
    {
        for(auto& connection : m_connections) {
            QObject::disconnect(connection);
        }
        m_connections.clear();

        if(m_recents == nullptr) {
            return;
        }

        m_connections.push_back(connect(m_recents, &RecentItems::signal_added, [this](const RecentItem& item) {
            addItem(item);
        }));

        m_connections.push_back(connect(m_recents, &RecentItems::signal_cleared, [this]() {
            q->clear();
        }));

        m_connections.push_back(connect(m_recents, &RecentItems::signal_removed, [this](const QString& item) {
            const QList<QListWidgetItem*> items{findItems(item, Qt::MatchExactly)};
            for(auto* item : items) {
                q->removeItemWidget(item);
            }
        }));
    }

    void addItem(const RecentItem& recentItem)
    {
        QListWidgetItem* item{new QListWidgetItem()};
        dialog::RecentItemWidget* button{new dialog::RecentItemWidget(recentItem)};

        item->setToolTip(recentItem.getDisplayName()); // TODO?

        item->setSizeHint(button->sizeHint());
        q->addItem(item);
        q->setItemWidget(item, button);
    }

    RecentJobsList* q;
    RecentItems* m_recents;
    std::vector<QMetaObject::Connection> m_connections;
};

RecentJobsList::RecentJobsList(QWidget* parent) : QListWidget(parent), d{std::make_unique<RecentJobsList::RecentJobsListImpl>(this)}
{
}

void RecentJobsList::setRecentItems(RecentItems* recents)
{
    d->setRecentItems(recents);
}

RecentItems* RecentJobsList::getRecentItems()
{
    return d->getRecentItems();
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
        // TODO remove item
    }
}

}

}
