// Copyright (c) 2016, The Qt Company Ltd.
// Contact: https://www.qt.io/licensing/
//
// Modifications Copyright (c) 2016, Sam Twidale.
// Contact: https://samcodes.co.uk/contact/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//   * Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in
//     the documentation and/or other materials provided with the
//     distribution.
//   * Neither the name of The Qt Company Ltd nor the names of its
//     contributors may be used to endorse or promote products derived
//     from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."

#include "flowlayout.h"

#include <QWidget>

namespace geometrize
{

namespace layout
{

FlowLayout::FlowLayout(const int margin, const int hSpacing, const int vSpacing) : m_hSpace(hSpacing), m_vSpace(vSpacing)
{
    setContentsMargins(margin, margin, margin, margin);
}

FlowLayout::~FlowLayout()
{
    while (QLayoutItem* item = takeAt(0)) {
        delete item;
        item = nullptr;
    }
}

int FlowLayout::horizontalSpacing() const
{
    if (m_hSpace >= 0) {
        return m_hSpace;
    } else {
        return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
    }
}

int FlowLayout::verticalSpacing() const
{
    if (m_vSpace >= 0) {
        return m_vSpace;
    } else {
        return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
    }
}

void FlowLayout::addItem(QLayoutItem* item)
{
    m_itemList.append(item);
}

Qt::Orientations FlowLayout::expandingDirections() const
{
    return Qt::Horizontal;
}

bool FlowLayout::hasHeightForWidth() const
{
    return true;
}

int FlowLayout::heightForWidth(const int width) const
{
    return doLayout(QRect(0, 0, width, 0), true);
}

int FlowLayout::count() const
{
    return m_itemList.size();
}

QLayoutItem* FlowLayout::itemAt(const int index) const
{
    return m_itemList.value(index);
}

QSize FlowLayout::minimumSize() const
{
    QSize size;
    QLayoutItem* item;
    foreach (item, m_itemList) {
        size = size.expandedTo(item->minimumSize());
    }

    int left = 0;
    int right = 0;
    int bottom = 0;
    int top = 0;
    getContentsMargins(&left, &top, &right, &bottom);

    size += QSize(left + right, bottom + top);
    return size;
}

void FlowLayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);
    doLayout(rect, false);
}

QSize FlowLayout::sizeHint() const
{
    return minimumSize();
}

QLayoutItem* FlowLayout::takeAt(const int index)
{
    if (index >= 0 && index < m_itemList.size()) {
        return m_itemList.takeAt(index);
    } else {
        return 0;
    }
}

int FlowLayout::doLayout(const QRect &rect, const bool testOnly) const
{
    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
    int x{effectiveRect.x()};
    int y{effectiveRect.y()};
    int lineHeight{0};

    QLayoutItem* item;
    foreach (item, m_itemList) {
        const QWidget* const wid{item->widget()};
        if(wid->isHidden()) {
            continue;
        }

        int spaceX{horizontalSpacing()};
        if (spaceX == -1) {
            spaceX = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);
        }
        int spaceY{verticalSpacing()};
        if (spaceY == -1) {
            spaceY = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);
        }
        int nextX{x + item->sizeHint().width() + spaceX};
        if (nextX - spaceX > effectiveRect.right() && lineHeight > 0) {
            x = effectiveRect.x();
            y = y + lineHeight + spaceY;
            nextX = x + item->sizeHint().width() + spaceX;
            lineHeight = 0;
        }

        if (!testOnly) {
            item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));
        }

        x = nextX;
        lineHeight = qMax(lineHeight, item->sizeHint().height());
    }
    return y + lineHeight - rect.y() + bottom;
}

int FlowLayout::smartSpacing(const QStyle::PixelMetric pm) const
{
    QObject* parent{this->parent()};
    if (!parent) {
        return -1;
    } else if (parent->isWidgetType()) {
        QWidget* pw{static_cast<QWidget*>(parent)};
        return pw->style()->pixelMetric(pm, 0, pw);
    } else {
        return static_cast<QLayout*>(parent)->spacing();
    }
}

}

}
