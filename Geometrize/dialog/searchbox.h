#pragma once

#include <memory>

#include <QLineEdit>
#include <QStringList>

namespace geometrize
{

namespace dialog
{

/**
 * @brief The SearchBox class is a line edit that facilitates searching job templates with an autocomplete drop-down list.
 */
class SearchBox : public QLineEdit
{
    Q_OBJECT

public:
    explicit SearchBox(QWidget* parent = 0);

    void setCompletionList(const QStringList& completionList);
    void addToCompletionList(const QString& item);

private:
    class SearchBoxImpl;
    std::unique_ptr<SearchBoxImpl> d;
};

}

}
