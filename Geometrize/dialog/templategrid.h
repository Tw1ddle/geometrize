#pragma once

#include <memory>

#include <QString>
#include <QWidget>

namespace geometrize
{

namespace dialog
{

/**
 * @brief The TemplateGrid class models the UI for a dynamic grid of project templates.
 */
class TemplateGrid : public QWidget
{
    Q_OBJECT
public:
    explicit TemplateGrid(QWidget *parent = 0);

signals:
    void signal_templateLoaded(QString, bool);

private:
    class TemplateGridImpl;
    std::unique_ptr<TemplateGridImpl> d;
};

}

}
