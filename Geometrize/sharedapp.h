#pragma once

#include <memory>

#include <QObject>

class QGraphicsScene;
class QPixmap;
class QString;
class QWidget;

namespace geometrize
{

class RecentItems;

/**
 * @brief The SharedApp class is a lazy singleton that contains common core functionality e.g. implementations of UI actions that can be triggered from multiple places.
 */
class SharedApp : public QObject
{
    Q_OBJECT
public:
    static SharedApp& get();

    SharedApp& operator=(const SharedApp&) = delete;
    SharedApp(const SharedApp&) = delete;

    // Common UI actions
    static void openAboutPage(QWidget* parent);
    static void openPreferences(QWidget* parent);
    static int openQuitDialog(QWidget* parent);
    static void openTechnicalSupport();
    static void openOnlineTutorials();

    void createImageJob(QWidget* parent, const QString& displayName, const QPixmap& pixmap);

    // Recent files
    RecentItems& getRecentFiles();

    // File picking actions
    QPixmap openPixmap(QWidget* parent, const QString& imagePath);
    void saveImage(QWidget* parent);
    static QString getImagePath(QWidget* parent);

signals:

private:
    class SharedAppImpl;
    std::unique_ptr<SharedAppImpl> d;

    SharedApp();
    ~SharedApp();
};

}
