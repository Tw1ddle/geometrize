// MIT License
//
// Copyright (c) 2019 ThisIsClark
// Modifications (c) 2021 Sam Twidale (https://samcodes.co.uk/)
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

#include "dialog/rangeslider.h"

#include <QDebug>

namespace
{

const int scHandleSideLength = 11;
const int scSliderBarHeight = 5;
const int scLeftRightMargin = 1;

}

namespace geometrize
{

namespace dialog
{

RangeSlider::RangeSlider(QWidget* aParent)
    : QWidget(aParent),
      mMinimum(0),
      mMaximum(100),
      mLowerValue(0),
      mUpperValue(100),
      mFirstHandlePressed(false),
      mSecondHandlePressed(false),
      mInterval(mMaximum - mMinimum),
      mBackgroudColorEnabled(QColor(0x1E, 0x90, 0xFF)),
      mBackgroudColorDisabled(Qt::darkGray),
      mBackgroudColor(mBackgroudColorEnabled),
      orientation(Qt::Horizontal)
{
    setMouseTracking(true);
}

RangeSlider::RangeSlider(Qt::Orientation ori, Options t, QWidget* aParent)
    : QWidget(aParent),
      mMinimum(0),
      mMaximum(100),
      mLowerValue(0),
      mUpperValue(100),
      mFirstHandlePressed(false),
      mSecondHandlePressed(false),
      mInterval(mMaximum - mMinimum),
      mBackgroudColorEnabled(QColor(0x1E, 0x90, 0xFF)),
      mBackgroudColorDisabled(Qt::darkGray),
      mBackgroudColor(mBackgroudColorEnabled),
      orientation(ori),
      type(t)
{
    setMouseTracking(true);
}

void RangeSlider::paintEvent(QPaintEvent* aEvent)
{
    Q_UNUSED(aEvent);
    QPainter painter(this);

    // Background
    QRectF backgroundRect;
    if(orientation == Qt::Horizontal)
        backgroundRect = QRectF(scLeftRightMargin, (height() - scSliderBarHeight) / 2, width() - scLeftRightMargin * 2, scSliderBarHeight);
    else
        backgroundRect = QRectF((width() - scSliderBarHeight) / 2, scLeftRightMargin, scSliderBarHeight, height() - scLeftRightMargin*2);

    QPen pen(Qt::gray, 0.8);
    painter.setPen(pen);
    QBrush backgroundBrush(QColor(0xD0, 0xD0, 0xD0));
    painter.setBrush(backgroundBrush);
    painter.drawRoundedRect(backgroundRect, 1, 1);

    // First value handle rect
    pen.setColor(Qt::darkGray);
    pen.setWidth(0.5);
    painter.setPen(pen);
    painter.setRenderHint(QPainter::Antialiasing);
    QBrush handleBrush(QColor(0xFA, 0xFA, 0xFA));
    painter.setBrush(handleBrush);
    QRectF leftHandleRect = firstHandleRect();
    if(type.testFlag(LeftHandle))
        painter.drawRoundedRect(leftHandleRect, 2, 2);

    // Second value handle rect
    QRectF rightHandleRect = secondHandleRect();
    if(type.testFlag(RightHandle))
        painter.drawRoundedRect(rightHandleRect, 2, 2);

    // Handles
    painter.setRenderHint(QPainter::Antialiasing, false);
    QRectF selectedRect(backgroundRect);
    if(orientation == Qt::Horizontal) {
        selectedRect.setLeft((type.testFlag(LeftHandle) ? leftHandleRect.right() : leftHandleRect.left()) + 0.5);
        selectedRect.setRight((type.testFlag(RightHandle) ? rightHandleRect.left() : rightHandleRect.right()) - 0.5);
    } else {
        selectedRect.setTop((type.testFlag(LeftHandle) ? leftHandleRect.bottom() : leftHandleRect.top()) + 0.5);
        selectedRect.setBottom((type.testFlag(RightHandle) ? rightHandleRect.top() : rightHandleRect.bottom()) - 0.5);
    }
    QBrush selectedBrush(mBackgroudColor);
    painter.setBrush(selectedBrush);
    painter.drawRect(selectedRect);
}

QRectF RangeSlider::firstHandleRect() const
{
    float percentage = (mLowerValue - mMinimum) * 1.0 / mInterval;
    return handleRect(percentage * validLength() + scLeftRightMargin);
}

QRectF RangeSlider::secondHandleRect() const
{
    float percentage = (mUpperValue - mMinimum) * 1.0 / mInterval;
    return handleRect(percentage * validLength() + scLeftRightMargin + (type.testFlag(LeftHandle) ? scHandleSideLength : 0));
}

QRectF RangeSlider::handleRect(int aValue) const
{
    if(orientation == Qt::Horizontal)
        return QRect(aValue, (height()-scHandleSideLength) / 2, scHandleSideLength, scHandleSideLength);
    else
        return QRect((width()-scHandleSideLength) / 2, aValue, scHandleSideLength, scHandleSideLength);
}

void RangeSlider::mousePressEvent(QMouseEvent* aEvent)
{
    if(aEvent->button() & Qt::RightButton)
    {
        const int min = GetMinimum();
        const int max = GetMaximum();
        const int lower = GetLowerValue();
        const int upper = GetUpperValue();

        // Flips the space in the slider
        setLowerValue(min + (max - upper));
        setUpperValue(max - (lower - min));
    }

    if(aEvent->buttons() & Qt::LeftButton)
    {
        int posCheck, posMax, posValue, firstHandleRectPosValue, secondHandleRectPosValue;
        posCheck = (orientation == Qt::Horizontal) ? aEvent->pos().y() : aEvent->pos().x();
        posMax = (orientation == Qt::Horizontal) ? height() : width();
        posValue = (orientation == Qt::Horizontal) ? aEvent->pos().x() : aEvent->pos().y();
        firstHandleRectPosValue = (orientation == Qt::Horizontal) ? firstHandleRect().x() : firstHandleRect().y();
        secondHandleRectPosValue = (orientation == Qt::Horizontal) ? secondHandleRect().x() : secondHandleRect().y();

        mSecondHandlePressed = secondHandleRect().contains(aEvent->pos());
        mFirstHandlePressed = !mSecondHandlePressed && firstHandleRect().contains(aEvent->pos());
        if(mFirstHandlePressed)
        {
            mDelta = posValue - (firstHandleRectPosValue + scHandleSideLength / 2);
        }
        else if(mSecondHandlePressed)
        {
            mDelta = posValue - (secondHandleRectPosValue + scHandleSideLength / 2);
        }

        else if(posCheck >= 2
                && posCheck <= posMax - 2)
        {
            int step = mInterval / 10 < 1 ? 1 : mInterval / 10;
            if(posValue < firstHandleRectPosValue)
                setLowerValue(mLowerValue - step);
            else if(posValue > secondHandleRectPosValue + scHandleSideLength)
                setUpperValue(mUpperValue + step);
            else if(((posValue > firstHandleRectPosValue + scHandleSideLength) || !type.testFlag(LeftHandle))
                    && ((posValue < secondHandleRectPosValue) || !type.testFlag(RightHandle)))
            {
                if(type.testFlag(DoubleHandles))
                    if(posValue - (firstHandleRectPosValue + scHandleSideLength) <
                            (secondHandleRectPosValue - (firstHandleRectPosValue + scHandleSideLength)) / 2)
                        setLowerValue((mLowerValue + step < mUpperValue) ? mLowerValue + step : mUpperValue);
                    else
                        setUpperValue((mUpperValue - step > mLowerValue) ? mUpperValue - step : mLowerValue);
                else if(type.testFlag(LeftHandle))
                    setLowerValue((mLowerValue + step < mUpperValue) ? mLowerValue + step : mUpperValue);
                else if(type.testFlag(RightHandle))
                    setUpperValue((mUpperValue - step > mLowerValue) ? mUpperValue - step : mLowerValue);
            }
        }
    }
}

void RangeSlider::mouseMoveEvent(QMouseEvent* aEvent)
{
    if(aEvent->buttons() & Qt::LeftButton)
    {
        int posValue, firstHandleRectPosValue, secondHandleRectPosValue;
        posValue = (orientation == Qt::Horizontal) ? aEvent->pos().x() : aEvent->pos().y();
        firstHandleRectPosValue = (orientation == Qt::Horizontal) ? firstHandleRect().x() : firstHandleRect().y();
        secondHandleRectPosValue = (orientation == Qt::Horizontal) ? secondHandleRect().x() : secondHandleRect().y();

        if(mFirstHandlePressed && type.testFlag(LeftHandle))
        {
            if(posValue - mDelta + scHandleSideLength / 2 <= secondHandleRectPosValue)
            {
                setLowerValue((posValue - mDelta - scLeftRightMargin - scHandleSideLength / 2) * 1.0 / validLength() * mInterval + mMinimum);
            }
            else
            {
                setLowerValue(mUpperValue);
            }
        }
        else if(mSecondHandlePressed && type.testFlag(RightHandle))
        {
            if(firstHandleRectPosValue + scHandleSideLength * (type.testFlag(DoubleHandles) ? 1.5 : 0.5) <= posValue - mDelta)
            {
                setUpperValue((posValue - mDelta - scLeftRightMargin - scHandleSideLength / 2 - (type.testFlag(DoubleHandles) ? scHandleSideLength : 0)) * 1.0 / validLength() * mInterval + mMinimum);
            }
            else
            {
                setUpperValue(mLowerValue);
            }
        }
    }
}

void RangeSlider::mouseReleaseEvent(QMouseEvent* aEvent)
{
    Q_UNUSED(aEvent);

    mFirstHandlePressed = false;
    mSecondHandlePressed = false;
}

void RangeSlider::changeEvent(QEvent* aEvent)
{
    if(aEvent->type() == QEvent::EnabledChange)
    {
        if(isEnabled())
        {
            mBackgroudColor = mBackgroudColorEnabled;
        }
        else
        {
            mBackgroudColor = mBackgroudColorDisabled;
        }
        update();
    }
}

QSize RangeSlider::minimumSizeHint() const
{
    return QSize(scHandleSideLength * 2 + scLeftRightMargin * 2, scHandleSideLength);
}

int RangeSlider::GetMinimum() const
{
    return mMinimum;
}

void RangeSlider::SetMinimum(int aMinimum)
{
    setMinimum(aMinimum);
}

int RangeSlider::GetMaximum() const
{
    return mMaximum;
}

void RangeSlider::SetMaximum(int aMaximum)
{
    setMaximum(aMaximum);
}

int RangeSlider::GetLowerValue() const
{
    return mLowerValue;
}

void RangeSlider::SetLowerValue(int aLowerValue)
{
    setLowerValue(aLowerValue);
}

int RangeSlider::GetUpperValue() const
{
    return mUpperValue;
}

void RangeSlider::SetUpperValue(int aUpperValue)
{
    setUpperValue(aUpperValue);
}

void RangeSlider::setLowerValue(int aLowerValue)
{
    if(aLowerValue > mMaximum)
    {
        aLowerValue = mMaximum;
    }

    if(aLowerValue < mMinimum)
    {
        aLowerValue = mMinimum;
    }

    mLowerValue = aLowerValue;
    emit lowerValueChanged(mLowerValue);

    update();
}

void RangeSlider::setUpperValue(int aUpperValue)
{
    if(aUpperValue > mMaximum)
    {
        aUpperValue = mMaximum;
    }

    if(aUpperValue < mMinimum)
    {
        aUpperValue = mMinimum;
    }

    mUpperValue = aUpperValue;
    emit upperValueChanged(mUpperValue);

    update();
}

void RangeSlider::setMinimum(int aMinimum)
{
    if(aMinimum <= mMaximum)
    {
        mMinimum = aMinimum;
    }
    else
    {
        int oldMax = mMaximum;
        mMinimum = oldMax;
        mMaximum = aMinimum;
    }
    mInterval = mMaximum - mMinimum;
    update();

    setLowerValue(mMinimum);
    setUpperValue(mMaximum);

    emit rangeChanged(mMinimum, mMaximum);
}

void RangeSlider::setMaximum(int aMaximum)
{
    if(aMaximum >= mMinimum)
    {
        mMaximum = aMaximum;
    }
    else
    {
        int oldMin = mMinimum;
        mMaximum = oldMin;
        mMinimum = aMaximum;
    }
    mInterval = mMaximum - mMinimum;
    update();

    setLowerValue(mMinimum);
    setUpperValue(mMaximum);

    emit rangeChanged(mMinimum, mMaximum);
}

int RangeSlider::validLength() const
{
    int len = (orientation == Qt::Horizontal) ? width() : height();
    return len - scLeftRightMargin * 2 - scHandleSideLength * (type.testFlag(DoubleHandles) ? 2 : 1);
}

void RangeSlider::SetRange(int aMinimum, int mMaximum)
{
    setMinimum(aMinimum);
    setMaximum(mMaximum);
}

}

}
