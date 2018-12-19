#pragma once

#include <memory>
#include <string>

#include <QWidget>

class QEvent;

namespace chaiscript
{
class ChaiScript;
}

namespace geometrize
{

namespace dialog
{

/**
 * @brief The CommandLineEdit class models the UI for the command line in a graphical script console.
 */
class CommandLineEdit : public QWidget
{
    Q_OBJECT

public:
    explicit CommandLineEdit(QWidget* parent = nullptr);
    virtual ~CommandLineEdit();

    /**
     * @brief clearHistory Wipe the history of commands entered this session.
     */
    void clearHistory();

    /**
     * @brief getHistory Gets the history of commands entered this session, oldest first.
     * @return The history of commands entered this session.
     */
    std::vector<std::string> getHistory() const;

    /**
     * @brief setHistory Sets the history of commands.
     * @param history The command history to use.
     */
    void setHistory(const std::vector<std::string>& history);

    /**
     * @brief setCompletionList Sets the autocomplete list of commands.
     * @param completionList The completion list to use.
     */
    void setCompletionList(const std::vector<std::string>& completionList);

signals:
    /**
     * @brief signal_commandSubmitted is fired when a command is submitted by the user (e.g. after pressing return).
     */
    void signal_commandSubmitted(const std::string&);

    /**
     * @brief signal_historyStepped is fired when the user steps through the command line history (e.g. when pressing the up arrow).
     */
    void signal_historyStepped(const std::string&);

protected:
    void changeEvent(QEvent*) override;

private:
    virtual void keyPressEvent(QKeyEvent* e) override;
    virtual bool focusNextPrevChild(bool next) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;

    class CommandLineEditImpl;
    std::unique_ptr<CommandLineEditImpl> d;
};

}

}
