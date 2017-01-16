#include "commandlineedit.h"
#include "ui_commandlineedit.h"

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
    CommandLineEditImpl(CommandLineEdit* pQ, Ui::CommandLineEdit* pUi) : q{pQ}, ui{pUi}, m_historyIndex{0}
    {
        ui->setupUi(q);
    }
    CommandLineEditImpl operator=(const CommandLineEditImpl&) = delete;
    CommandLineEditImpl(const CommandLineEditImpl&) = delete;
    ~CommandLineEditImpl() = default;

    // TODO add completion events
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
            default:
                break;
        }
    }

    void stepHistory(const int steps)
    {
        m_historyIndex += steps;
        m_historyIndex = qBound(0, m_historyIndex, std::max(0, static_cast<int>(m_history.size())));

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
        m_historyIndex = static_cast<int>(m_history.size());
    }

    void clearHistory()
    {
        m_history.clear();
        m_historyIndex = 0;
    }

private:
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

    CommandLineEdit* q;
    Ui::CommandLineEdit* ui;

    int m_historyIndex;
    std::vector<std::string> m_history;
};

CommandLineEdit::CommandLineEdit(QWidget *parent) : QWidget(parent), ui(new Ui::CommandLineEdit), d{std::make_unique<CommandLineEdit::CommandLineEditImpl>(this, ui)}
{
}

CommandLineEdit::~CommandLineEdit()
{
    delete ui;
}

void CommandLineEdit::keyPressEvent(QKeyEvent* e)
{
    QWidget::keyPressEvent(e);

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

}

}
