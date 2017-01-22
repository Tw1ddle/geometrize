#pragma once

#include <memory>

#include <QListWidget>

namespace geometrize
{
class RecentItems;
}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The RecentJobsList class models the UI for a list of recently opened jobs.
 */
class RecentJobsList : public QListWidget
{
    Q_OBJECT
public:
    explicit RecentJobsList(QWidget* parent = nullptr);

    /**
     * @brief setRecentItems Sets the items that the list will keep track of.
     * @param items The list of items, the recent jobs list does not take ownership of this.
     */
    void setRecentItems(RecentItems* items);

    /**
     * @brief getDisplayNameForJobPath Extracts a display name suitable for displaying to the user from the job URL/file path.
     * @param url The URL pointing to the job (could be a local file, a webpage, a file from a template or sample, whatever).
     * @return A display name for the job, suitable for showing to the user in the recent jobs list.
     */
    static QString getDisplayNameForJobPath(const QUrl& url);

private:
    virtual void keyPressEvent(QKeyEvent* e) override;
    virtual void contextMenuEvent(QContextMenuEvent* e) override;

    class RecentJobsListImpl;
    std::unique_ptr<RecentJobsListImpl> d;
};

}

}
