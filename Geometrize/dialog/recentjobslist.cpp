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
        q->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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

    static QString getDisplayNameForJobPath(const QUrl& url)
    {
        if(url.isEmpty()) {
            return tr("Empty URL");
        }

        // Look for a filename first
        const QString fileName{url.fileName(QUrl::FullyDecoded)};
        if(!fileName.isEmpty() ) {
            return fileName;
        }

        // Otherwise return only a trimmed part of the URL's path
        return url.path(QUrl::FullyDecoded).rightJustified(50, '.', false);
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
            clear();
        }));

        m_connections.push_back(connect(m_recents, &RecentItems::signal_removed, [this](const QString& key) {
            removeItem(key);
        }));
    }

    void addItem(const RecentItem& recentItem) const
    {
        dialog::RecentItemWidget* button{new dialog::RecentItemWidget(recentItem)};
        QListWidgetItem* item{new QListWidgetItem()};
        item->setToolTip(recentItem.getKey());
        item->setSizeHint(button->sizeHint());
        item->setData(Qt::UserRole, recentItem.getKey());
        q->addItem(item);
        q->setItemWidget(item, button);
    }

    void removeItem(const QString& key) const
    {
        for(int i = 0; i < q->count(); i++) {
            if(q->item(i)->data(Qt::UserRole).toString() == key) {
                q->takeItem(i);
                return;
            }
        }
    }

    void clear() const
    {
        q->clear();
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

QString RecentJobsList::getDisplayNameForJobPath(const QUrl &url)
{
    return RecentJobsListImpl::getDisplayNameForJobPath(url);
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
