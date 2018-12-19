#pragma once

#include <memory>

#include <QString>
#include <QWidget>

class QEvent;

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
    explicit TemplateGrid(QWidget* parent = nullptr);
    virtual ~TemplateGrid();

    /**
     * @brief loadTemplates Loads all of the available templates, creating buttons for each one in the grid.
     */
    void loadTemplates();

    /**
     * @brief setItemFilter Filters the visible items in the template item grid.
     * @param filter The filter string.
     */
    void setItemFilter(const QString& filter);

protected:
    void changeEvent(QEvent*) override;

signals:
    void signal_templateLoaded(QString, bool);

private:
    class TemplateGridImpl;
    std::unique_ptr<TemplateGridImpl> d;
};

}

}
