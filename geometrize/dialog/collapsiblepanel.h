#pragma once

#include <QWidget>

class QFrame;
class QLabel;
class QPushButton;

namespace geometrize
{

namespace dialog
{

/**
 * @brief The CollapsiblePanel class is a UI container panel that can be expanded/collapsed.
 */
class CollapsiblePanel : public QWidget
{
    Q_OBJECT
public:
    explicit CollapsiblePanel(QWidget* parent = nullptr);

    void setTitle (const QString& title);
    virtual QSize minimumSizeHint() const override;

public slots:
    void setExpanded(bool isExpanded);
    void toggleExpanded();

private:
    virtual void resizeEvent(QResizeEvent *) override;
    virtual bool event(QEvent* event) override;
    QFrame* getContentsPanel() const;

    QLabel* m_titleLabel;
    QPushButton* m_collapseButton;
    QFrame* m_titleFrame;
};

}

}
