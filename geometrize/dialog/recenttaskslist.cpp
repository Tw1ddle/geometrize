#include "recenttaskslist.h"

#include <cassert>

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QContextMenuEvent>
#include <QEvent>
#include <QMenu>

#include "task/taskutil.h"
#include "dialog/taskitemwidget.h"
#include "recents/recentitem.h"
#include "recents/recentitems.h"

namespace geometrize
{

namespace dialog
{

class RecentTasksList::RecentTasksListImpl
{
public:
    RecentTasksListImpl(RecentTasksList* pQ) : q{pQ}, m_recents{nullptr}
    {
        q->setSizeAdjustPolicy(QListWidget::AdjustToContents);
        q->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        populateUi();
    }

    RecentTasksListImpl operator=(const RecentTasksListImpl&) = delete;
    RecentTasksListImpl(const RecentTasksListImpl&) = delete;
    ~RecentTasksListImpl() = default;

    static QString getDisplayNameForTaskPath(const QUrl& url)
    {
        if(url.isEmpty()) {
            return "";
        }

        // Look for a filename first
        const QString fileName{url.fileName(QUrl::FullyDecoded)};
        if(!fileName.isEmpty() ) {
            return fileName;
        }

        // Otherwise return only a trimmed part of the URL's path
        return url.path(QUrl::FullyDecoded).rightJustified(50, '.', true);
    }

    void setRecentItems(RecentItems* recents)
    {
        m_recents = recents;
        q->clear();
        setupConnections();
        loadExistingItems();
    }

    void clearItemDataAndMenu() const
    {
        if(m_recents == nullptr) {
            return;
        }
        m_recents->clear();
    }

    void removeItemDataAndMenuItems(const QList<QListWidgetItem*>& items)
    {
        if(m_recents == nullptr) {
            return;
        }
        for(const QListWidgetItem* const item : items) {
            m_recents->remove(getMenuItemKey(item));
        }
    }

    void onLanguageChange()
    {
        populateUi();
    }

private:
    void populateUi()
    {

    }

    void loadExistingItems()
    {
        if(m_recents == nullptr) {
            return;
        }

        const QList<RecentItem> items{m_recents->getItems()};
        for(const RecentItem& item : items) {
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
        dialog::TaskItemWidget* button{new dialog::TaskItemWidget(recentItem.getKey(), getDisplayNameForTaskPath(recentItem.getDisplayName()),
        [](const QString& taskItemId) {
            geometrize::util::openTasks({taskItemId}, false);
        },
        [](const QString& taskItemId) {
            geometrize::getRecentItems().remove(taskItemId);
        })};
        QListWidgetItem* item{new QListWidgetItem()};
        item->setToolTip(recentItem.getKey());
        item->setSizeHint(button->sizeHint());
        setMenuItemKey(item, recentItem.getKey());
        q->addItem(item);
        q->setItemWidget(item, button);
    }

    void removeItem(const QString& key) const
    {
        for(int i = 0; i < q->count(); i++) {
            if(getMenuItemKey(q->item(i)) == key) {
                q->takeItem(i);
                return;
            }
        }
    }

    void clear() const
    {
        q->clear();
    }

    QString getMenuItemKey(const QListWidgetItem* const item) const
    {
        return item->data(Qt::UserRole).toString();
    }

    void setMenuItemKey(QListWidgetItem* item, const QString& key) const
    {
        item->setData(Qt::UserRole, key);
    }

    RecentTasksList* q;
    RecentItems* m_recents;
    std::vector<QMetaObject::Connection> m_connections;
};

RecentTasksList::RecentTasksList(QWidget* parent) : QListWidget(parent), d{std::make_unique<RecentTasksList::RecentTasksListImpl>(this)}
{
}

RecentTasksList::~RecentTasksList()
{
}

void RecentTasksList::setRecentItems(RecentItems* recents)
{
    d->setRecentItems(recents);
}

QString RecentTasksList::getDisplayNameForTaskPath(const QUrl &url)
{
    return RecentTasksListImpl::getDisplayNameForTaskPath(url);
}

void RecentTasksList::keyPressEvent(QKeyEvent* e)
{
    QListWidget::keyPressEvent(e);

    const QList<QListWidgetItem*> items{selectedItems()};

    if(e->matches(QKeySequence::Copy)) {
        QStringList strings;
        for(const QListWidgetItem* item : items) {
            strings.push_back(item->text());
        }
        QApplication::clipboard()->setText(strings.join("\n"));
    } else if(e->key() == Qt::Key_Delete || e->key() == Qt::Key_Backspace) {
        d->removeItemDataAndMenuItems(items);
    }
}

void RecentTasksList::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        d->onLanguageChange();
    }
    QWidget::changeEvent(event);
}

}

}
