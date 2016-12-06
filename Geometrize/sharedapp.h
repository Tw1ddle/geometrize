#ifndef SHAREDAPP_H
#define SHAREDAPP_H

#include <memory>

class QGraphicsScene;
class QPixmap;
class QString;
class QWidget;

namespace geometrize
{

class SharedAppImpl;

/**
 * @brief The SharedApp class is a singleton that contains common core functionality e.g. implementations of UI actions that can be triggered from multiple places.
 */
class SharedApp
{
public:
    SharedApp();
    SharedApp& operator=(const SharedApp&) = delete;
    SharedApp(const SharedApp&) = delete;
    ~SharedApp();

   void createImageJob(QWidget* parent, const QPixmap& pixmap) const;

   void openAboutPage(QWidget* parent) const;
   void openPreferences(QWidget* parent) const;
   int openQuitDialog(QWidget* parent) const;

   void openTechnicalSupport() const;
   void openOnlineTutorials() const;

   QPixmap openPixmap(QWidget* parent, const QString& imagePath) const;
   void saveImage(QWidget* parent) const;

   QString getImagePath(QWidget* parent) const;

   void addRecentFile(const QString& filePath) const;
   void removeRecentFile(const QString& filePath) const;
   void clearRecentFiles() const;

private:
    std::unique_ptr<SharedAppImpl> d;
};

}

#endif // SHAREDAPP_H
