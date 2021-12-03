#include "commandlineedit.h"
#include "ui_commandlineedit.h"

#include <algorithm>
#include <string>
#include <vector>

#include <QKeyEvent>

namespace geometrize
{

namespace dialog
{

class CommandLineEdit::CommandLineEditImpl
{
public:
    CommandLineEditImpl(CommandLineEdit* pQ) : q{pQ}, ui{std::make_unique<Ui::CommandLineEdit>()}, m_historyIndex{0}
    {
        ui->setupUi(q);
        populateUi();
    }
    CommandLineEditImpl operator=(const CommandLineEditImpl&) = delete;
    CommandLineEditImpl(const CommandLineEditImpl&) = delete;
    ~CommandLineEditImpl() = default;

    void keyPressEvent(const int key)
    {
        switch(key) {
            case Qt::Key_Up:
                stepHistory(-1);
                break;
            case Qt::Key_Down:
                stepHistory(1);
                break;
            case Qt::Key_Return:
                submitCommand();
                break;
            case Qt::Key_Tab:
                setCurrentCommand(autoCompleteForCurrentInput());
                break;
            default:
                break;
        }
    }

    void stepHistory(const int steps)
    {
        m_historyIndex = static_cast<std::size_t>(std::min(static_cast<std::int64_t>(m_history.size()),
                                                           std::max(static_cast<std::int64_t>(0), static_cast<std::int64_t>(m_historyIndex) + steps)));

        if(m_historyIndex == m_history.size()) {
            setCurrentCommand("");
        } else if(!m_history.empty()) {
            setCurrentCommand(m_history.at(m_historyIndex));
        }

        emit q->signal_historyStepped(getCurrentCommand());
    }

    void submitCommand()
    {
        const std::string command{getCurrentCommand()};
        m_history.push_back(command);
        clear();
        m_historyIndex = static_cast<int>(m_history.size());
        emit q->signal_commandSubmitted(command);
    }

    std::vector<std::string> getHistory()
    {
        return m_history;
    }

    void setHistory(const std::vector<std::string>& history)
    {
        m_history = history;
        m_historyIndex = m_history.size();

        //QStringList words;
        //for(const std::string& word : history) {
        //    words.push_back(QString::fromStdString(word));
        //}
        //ui->lineEdit->setCompletionList(words);
    }

    void setCompletionList(const std::vector<std::string>& completionList)
    {
        QStringList words;
        for(const std::string& word : completionList) {
            words.push_back(QString::fromStdString(word));
        }
        ui->lineEdit->setCompletionList(words);
    }

    void clearHistory()
    {
        m_history.clear();
        m_historyIndex = 0;
    }

    void mouseDoubleClickEvent()
    {

    }

    void onLanguageChange()
    {
        populateUi();
        ui->retranslateUi(q);
    }

private:
    void populateUi()
    {

    }

    std::string getCurrentCommand() const
    {
        return ui->lineEdit->text().toStdString();
    }

    void setCurrentCommand(const std::string& command)
    {
        ui->lineEdit->setText(QString::fromStdString(command));
    }

    void clear()
    {
        ui->lineEdit->clear();
    }

    std::string autoCompleteForCurrentInput()
    {
        // TODO get possible autocompletes
        const std::string current{getCurrentCommand()};
        return "";
    }

    CommandLineEdit* q;
    std::unique_ptr<Ui::CommandLineEdit> ui;

    std::size_t m_historyIndex;
    std::vector<std::string> m_history;
};

CommandLineEdit::CommandLineEdit(QWidget *parent) : QWidget(parent), d{std::make_unique<CommandLineEdit::CommandLineEditImpl>(this)}
{
}

CommandLineEdit::~CommandLineEdit()
{
}

void CommandLineEdit::keyPressEvent(QKeyEvent* e)
{
    d->keyPressEvent(e->key());
}

void CommandLineEdit::clearHistory()
{
    d->clearHistory();
}

std::vector<std::string> CommandLineEdit::getHistory() const
{
    return d->getHistory();
}

void CommandLineEdit::setHistory(const std::vector<std::string>& history)
{
    d->setHistory(history);
}

void CommandLineEdit::setCompletionList(const std::vector<std::string>& completionList)
{
    d->setCompletionList(completionList);
}

bool CommandLineEdit::focusNextPrevChild(const bool /*next*/)
{
    return false; // This ensures tab keypresses do not cause the widget to lose focus
}

void CommandLineEdit::mouseDoubleClickEvent(QMouseEvent* /*event*/)
{
    d->mouseDoubleClickEvent();
}

void CommandLineEdit::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        d->onLanguageChange();
    }
    QWidget::changeEvent(event);
}

}

}
