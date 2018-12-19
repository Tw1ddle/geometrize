#pragma once

#include <memory>

#include <QLineEdit>
#include <QStringList>

class QEvent;

namespace geometrize
{

namespace dialog
{

/**
 * @brief The CompletionBox class is a line edit that facilitates searching task templates with an autocomplete drop-down list.
 */
class CompletionBox : public QLineEdit
{
    Q_OBJECT

public:
    explicit CompletionBox(QWidget* parent = nullptr);
    virtual ~CompletionBox();

    void setCompletionList(const QStringList& completionList);
    void addToCompletionList(const QString& item);

protected:
    void changeEvent(QEvent*) override;

private:
    class CompletionBoxImpl;
    std::unique_ptr<CompletionBoxImpl> d;
};

}

}
