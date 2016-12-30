#pragma once

#include <memory>

#include <QList>
#include <QObject>
#include <QUrl>

class QGraphicsScene;
class QPixmap;
class QString;
class QWidget;

namespace geometrize
{
class ImageJobContext;
class RecentItems;
}

namespace geometrize
{

namespace app
{

// Common UI actions
void openAboutPage(QWidget* parent);
void openPreferences(QWidget* parent);
int openQuitDialog(QWidget* parent);
QUrl openGetUrlDialog(QWidget* parent);
void openTechnicalSupport();
void openOnlineTutorials();
void openTechnicalSupport();

// File picker
QPixmap openPixmap(QWidget* parent, const QString& imagePath);
void saveImage(QWidget* parent);
QString getImagePath(QWidget* parent);

// Image job creation/opening
void openJobs(const QStringList& urls);
ImageJobContext* createImageJob(QWidget* parent, const QString& displayName, const QPixmap& pixmap);
ImageJobContext* createImageJobAndUpdateRecents(QWidget* parent, const QUrl& url);

/**
 * @brief The SharedApp class is a lazy singleton that contains common core functionality.
 */
class SharedApp : public QObject
{
    Q_OBJECT
public:
    static SharedApp& get();
    SharedApp& operator=(const SharedApp&) = delete;
    SharedApp(const SharedApp&) = delete;

    // Recent files
    RecentItems& getRecentFiles();

private:
    class SharedAppImpl;
    std::unique_ptr<SharedAppImpl> d;

    SharedApp();
    ~SharedApp();
};

}

}
