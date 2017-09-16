#pragma once

#include <memory>

#include <QListWidget>

class QEvent;

namespace geometrize
{
class RecentItems;
}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The RecentTasksList class models the UI for a list of recently opened tasks.
 */
class RecentTasksList : public QListWidget
{
    Q_OBJECT

public:
    explicit RecentTasksList(QWidget* parent = nullptr);
    ~RecentTasksList();

    /**
     * @brief setRecentItems Sets the items that the list will keep track of.
     * @param items The list of items, the recent tasks list does not take ownership of this.
     */
    void setRecentItems(RecentItems* items);

    /**
     * @brief getDisplayNameForTaskPath Extracts a display name suitable for displaying to the user from the task URL/file path.
     * @param url The URL pointing to the task (could be a local file, a webpage, a file from a template or sample, whatever).
     * @return A display name for the task, suitable for showing to the user in the recent tasks list.
     */
    static QString getDisplayNameForTaskPath(const QUrl& url);

protected:
    void changeEvent(QEvent*) override;

private:
    virtual void keyPressEvent(QKeyEvent* e) override;

    class RecentTasksListImpl;
    std::unique_ptr<RecentTasksListImpl> d;
};

}

}
