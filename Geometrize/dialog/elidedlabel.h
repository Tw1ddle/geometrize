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

#pragma once

#include <QLabel>

namespace geometrize
{

namespace dialog
{

/**
 * @brief The ElidedLabel class is a label that elides text that does not fit within it.
 */
class ElidedLabel : public QLabel
{
public:
    ElidedLabel(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ElidedLabel(const QString& txt, QWidget* parent = 0, Qt::WindowFlags f = 0);
    ElidedLabel(const QString& txt, Qt::TextElideMode elideMode = Qt::ElideRight, QWidget* parent = 0, Qt::WindowFlags f = 0);

    /**
     * @brief getElideMode Get the elide mode currently used when displaying text.
     * @return The current elide mode.
     */
    Qt::TextElideMode getElideMode() const;

    /**
     * @brief setElideMode Set the elide mode used when displaying text.
     * @param elideMode The elide mode to set.
     */
    void setElideMode(Qt::TextElideMode elideMode);

    /**
     * @brief setText overrides QLabel::setText.
     */
    void setText(const QString&);

protected:
    void paintEvent(QPaintEvent*) override;
    void resizeEvent(QResizeEvent*) override;

private:
    /**
     * @brief cacheElidedText Caches the elided text, to avoid recomputing it every paint event.
     * @param w The width of the widget.
     */
    void cacheElidedText(int w);

    Qt::TextElideMode m_elideMode; ///< The current elision mode.
    QString m_elidedText; ///< The cached elided text.
};

}

}
