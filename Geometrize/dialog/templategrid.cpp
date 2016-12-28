#include "templategrid.h"

#include <assert.h>

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QGridLayout>
#include <QFileSystemWatcher>
#include <QMessageBox>
#include <QPushButton>
#include <QStyle>

#include "script/chaiscriptcreator.h"
#include "chaiscript/chaiscript.hpp"
#include "searchpaths.h"
#include "util.h"

namespace geometrize
{

namespace dialog
{

class FlowLayout : public QLayout
{
public:
    explicit FlowLayout(const int margin = -1, const int hSpacing = -1, const int vSpacing = -1) : m_hSpace(hSpacing), m_vSpace(vSpacing)
    {
        setContentsMargins(margin, margin, margin, margin);
    }

    ~FlowLayout()
    {
        while (QLayoutItem* item = takeAt(0)) {
            delete item;
            item = nullptr;
        }
    }

    void addItem(QLayoutItem* item) override
    {
        m_itemList.append(item);
    }

    int horizontalSpacing() const
    {
        if (m_hSpace >= 0) {
            return m_hSpace;
        } else {
            return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
        }
    }

    int verticalSpacing() const
    {
        if (m_vSpace >= 0) {
            return m_vSpace;
        } else {
            return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
        }
    }

    Qt::Orientations expandingDirections() const override
    {
        return 0;
    }

    bool hasHeightForWidth() const override
    {
        return true;
    }

    int heightForWidth(const int width) const override
    {
        return doLayout(QRect(0, 0, width, 0), true);
    }

    int count() const override
    {
        return m_itemList.size();
    }

    QLayoutItem* itemAt(const int index) const override
    {
        return m_itemList.value(index);
    }

    QSize minimumSize() const override
    {
        QSize size;
        QLayoutItem* item;
        foreach (item, m_itemList) {
            size = size.expandedTo(item->minimumSize());
        }

        size += QSize(2 * margin(), 2 * margin());
        return size;
    }

    void setGeometry(const QRect &rect) override
    {
        QLayout::setGeometry(rect);
        doLayout(rect, false);
    }

    QSize sizeHint() const override
    {
        return minimumSize();
    }

    QLayoutItem *takeAt(const int index) override
    {
        if (index >= 0 && index < m_itemList.size()) {
            return m_itemList.takeAt(index);
        } else {
            return 0;
        }
    }

private:
    int doLayout(const QRect &rect, const bool testOnly) const
    {
        int left, top, right, bottom;
        getContentsMargins(&left, &top, &right, &bottom);
        QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
        int x = effectiveRect.x();
        int y = effectiveRect.y();
        int lineHeight = 0;

        QLayoutItem *item;
        foreach (item, m_itemList) {
            QWidget *wid = item->widget();
            int spaceX = horizontalSpacing();
            if (spaceX == -1)
                spaceX = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);
            int spaceY = verticalSpacing();
            if (spaceY == -1)
                spaceY = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);
            int nextX = x + item->sizeHint().width() + spaceX;
            if (nextX - spaceX > effectiveRect.right() && lineHeight > 0) {
                x = effectiveRect.x();
                y = y + lineHeight + spaceY;
                nextX = x + item->sizeHint().width() + spaceX;
                lineHeight = 0;
            }

            if (!testOnly)
                item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));

            x = nextX;
            lineHeight = qMax(lineHeight, item->sizeHint().height());
        }
        return y + lineHeight - rect.y() + bottom;
    }

    int smartSpacing(const QStyle::PixelMetric pm) const
    {
        QObject* parent = this->parent();
        if (!parent) {
            return -1;
        } else if (parent->isWidgetType()) {
            QWidget* pw = static_cast<QWidget*>(parent);
            return pw->style()->pixelMetric(pm, 0, pw);
        } else {
            return static_cast<QLayout*>(parent)->spacing();
        }
    }

    QList<QLayoutItem*> m_itemList;
    int m_hSpace;
    int m_vSpace;
};

class TemplateItem : public QPushButton
{
public:
    TemplateItem(chaiscript::ChaiScript* const templateLoader, const QString& scriptPath) : QPushButton(), m_templateLoader{templateLoader}, m_scriptPath{scriptPath}
    {
        connect(this, &TemplateItem::clicked, [this]() {
            try {
                const std::string script{util::readFileAsString(m_scriptPath.toStdString())};
                m_templateLoader->eval(script);
            } catch (const std::string& s) {
                QMessageBox::warning(nullptr, QString::fromStdString(s), QString::fromStdString(s));
            } catch (const std::exception& e) {
              // This is the one what will be called in the specific throw() above
                 QMessageBox::warning(nullptr, QString(e.what()), QString(e.what()));
            } catch (...) {
                assert(0 && "Failed to evaluate template script");
            }
        });
    }

private:
    const QString m_scriptPath;
    chaiscript::ChaiScript* const m_templateLoader;
};

class TemplateGrid::TemplateGridImpl
{
public:
    TemplateGridImpl(TemplateGrid* pQ) : q{pQ}, m_layout{new FlowLayout(40, 32, 32)}, m_templateLoader{geometrize::script::createChaiScript()}
    {
        q->setLayout(m_layout);

        const std::string basePath{geometrize::searchpaths::getApplicationDirectoryPath()};
        const std::vector<std::string> paths{geometrize::searchpaths::getTemplateSearchPaths()};
        for(const std::string& path : paths) {
            const QString fullPath{QString::fromStdString(basePath + path)};
            m_fileWatch.addPath(fullPath);

            const std::vector<std::string> templates{util::getScriptsForPath(fullPath.toStdString())};

            for(const std::string& t : templates) {
                const bool result{addTemplateItem(QString::fromStdString(t))};
                emit q->signal_templateLoaded(QString::fromStdString(t), result);
            }
        }
    }

    TemplateGridImpl operator=(const TemplateGridImpl&) = delete;
    TemplateGridImpl(const TemplateGridImpl&) = delete;
    ~TemplateGridImpl() = default;

private:
    bool addTemplateItem(const QString& scriptPath)
    {
        m_layout->addWidget(new TemplateItem(m_templateLoader.get(), scriptPath));
        return true;
    }

    TemplateGrid* q;
    FlowLayout* m_layout;
    QFileSystemWatcher m_fileWatch;
    std::unique_ptr<chaiscript::ChaiScript> m_templateLoader;
};

TemplateGrid::TemplateGrid(QWidget *parent) :
    QWidget(parent),
    d{std::make_unique<TemplateGrid::TemplateGridImpl>(this)}
{
}

}

}
