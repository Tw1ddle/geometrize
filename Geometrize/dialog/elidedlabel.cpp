// The MIT License (MIT)
//
// Copyright (c) Yash (http://speedovation.com/)
//
// Modifications Copyright (c) 2017, Sam Twidale.
// Contact: http://samcodes.co.uk/contact/
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "elidedlabel.h"

#include <QPainter>
#include <QResizeEvent>

namespace geometrize
{

namespace dialog
{

class ElidedLabel::ElidedLabelImpl
{
public:
    ElidedLabelImpl(ElidedLabel* pQ, Qt::TextElideMode elideMode = Qt::ElideRight) : q{pQ}, m_elideMode{elideMode}
    {
    }

    Qt::TextElideMode getElideMode() const
    {
        return m_elideMode;
    }

    void setElideMode(const Qt::TextElideMode elideMode)
    {
        m_elideMode = elideMode;
        q->updateGeometry();
    }

    void setText(const QString& text)
    {
        cacheElidedText(text, q->geometry().width());
    }

    QString getElidedText() const
    {
        return m_elidedText;
    }

    void cacheElidedText(const QString& text, const int width)
    {
        m_elidedText = q->fontMetrics().elidedText(text, m_elideMode, width, Qt::TextShowMnemonic);
    }

private:
    Qt::TextElideMode m_elideMode; ///< The current elision mode.
    QString m_elidedText; ///< The cached elided text.
    ElidedLabel* q;
};

ElidedLabel::ElidedLabel(QWidget* parent, const Qt::WindowFlags flags) :
    QLabel(parent, flags),
    d{std::make_unique<ElidedLabel::ElidedLabelImpl>(this)}
{
}

ElidedLabel::ElidedLabel(const QString& text, QWidget* parent, const Qt::WindowFlags flags) :
    QLabel(text, parent, flags),
    d{std::make_unique<ElidedLabel::ElidedLabelImpl>(this)}
{
}

ElidedLabel::ElidedLabel(const QString& text, const Qt::TextElideMode elideMode, QWidget* parent, const Qt::WindowFlags flags) :
    QLabel(text, parent, flags),
    d{std::make_unique<ElidedLabel::ElidedLabelImpl>(this, elideMode)}
{
}

Qt::TextElideMode ElidedLabel::getElideMode() const
{
    return d->getElideMode();
}

void ElidedLabel::setElideMode(const Qt::TextElideMode elideMode)
{
    d->setElideMode(elideMode);
}

void ElidedLabel::setText(const QString& text)
{
    QLabel::setText(text);
    d->setText(text);
}

void ElidedLabel::resizeEvent(QResizeEvent* e)
{
    QLabel::resizeEvent(e);
    d->cacheElidedText(text(), e->size().width());
}

void ElidedLabel::paintEvent(QPaintEvent* e)
{
    if (d->getElideMode() == Qt::ElideNone) {
        QLabel::paintEvent(e);
    } else {
        QPainter p(this);
        p.drawText(0, 0, geometry().width(), geometry().height(), alignment(), d->getElidedText(), nullptr);
    }
}

}

}
