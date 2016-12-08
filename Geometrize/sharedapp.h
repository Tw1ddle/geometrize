#ifndef SHAREDAPP_H
#define SHAREDAPP_H

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

    void createImageJob(QWidget* parent, const QPixmap& pixmap) const;

    // Common UI actions
    void openAboutPage(QWidget* parent) const;
    void openPreferences(QWidget* parent) const;
    int openQuitDialog(QWidget* parent) const;
    void openTechnicalSupport() const;
    void openOnlineTutorials() const;

    // File picking actions
    QPixmap openPixmap(QWidget* parent, const QString& imagePath) const;
    void saveImage(QWidget* parent) const;
    QString getImagePath(QWidget* parent) const;

    // Recent files
    RecentItems& getRecentFiles();

signals:
    void signal_imageOpened(const QString& imagePath) const;

private:
    class SharedAppImpl;
    std::unique_ptr<SharedAppImpl> d;

    SharedApp();
    ~SharedApp();
};

}

#endif // SHAREDAPP_H
