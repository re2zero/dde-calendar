/*
 * Copyright (C) 2020 ~ 2021 Uniontech Software Technology Co.,Ltd.
 *
 * Author:     Ji XiangLong <jixianglong@uniontech.com>
 *
 * Maintainer: WangYu <wangyu@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "colorlabel.h"

#include <QImage>
#include <QRgb>
#include <QBitmap>

#include <cmath>

ColorLabel::ColorLabel(DWidget *parent)
    : DLabel(parent)
    , m_workToPick(/*false*/ true)
    , m_picking(true)
    , m_pressed(false)
{
    setMouseTracking(true);
    connect(this, &ColorLabel::clicked, this, [ = ] {
        if (m_picking && m_workToPick)
        {
            pickColor(m_clickedPos, true);
        }
    });
    setCursor(pickColorCursor());
}

//h∈(0, 360), s∈(0, 1), v∈(0, 1)
QColor ColorLabel::getColor(qreal h, qreal s, qreal v)
{
    int hi = int(h / 60) % 6;
    qreal f = h / 60 - hi;

    qreal p = v * (1 - s);
    qreal q = v * (1 - f * s);
    qreal t = v * (1 - (1 - f) * s);

    if (hi == 0) {
        return QColor(std::min(int(255 * p), 255), std::min(int(255 * q), 255), std::min(int(255 * v), 255));
    } else if (hi == 1) {
        return QColor(std::min(int(255 * t), 255), std::min(int(255 * p), 255), std::min(int(255 * v), 255));
    } else if (hi == 2) {
        return QColor(std::min(int(255 * v), 255), std::min(int(255 * p), 255), int(255 * q));
    } else if (hi == 3) {
        return QColor(std::min(int(255 * v), 255), std::min(int(255 * t), 255), std::min(int(255 * p), 255));
    } else if (hi == 4) {
        return QColor(std::min(int(255 * q), 255), std::min(int(255 * v), 255), std::min(int(255 * p), 255));
    } else {
        return QColor(std::min(int(255 * p), 255), std::min(int(255 * v), 255), std::min(int(255 * t), 255));
    }

}

void ColorLabel::setHue(int hue)
{
    m_hue = hue;
    update();
}

void ColorLabel::pickColor(QPoint pos, bool picked)
{
    if (!rect().contains(pos)) {
        return;
    }

    QPixmap pickPixmap;
    pickPixmap = this->grab(QRect(0, 0, this->width(), this->height()));
    QImage pickImg = pickPixmap.toImage();

    if (!pickImg.isNull()) {
        QRgb pickRgb = pickImg.pixel(pos);
        m_pickedColor = QColor(qRed(pickRgb), qGreen(pickRgb), qBlue(pickRgb));
    } else {
        m_pickedColor = QColor(0, 0, 0);
    }

    if (picked) {
        emit signalpickedColor(m_pickedColor);
    } else {
        emit signalPreViewColor(m_pickedColor);
    }
}

void ColorLabel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QImage backgroundImage(this->width(), this->height(), QImage::Format_ARGB32);
    QColor penColor;
    for (qreal s = 0; s < this->width(); s++) {
        for (qreal v = 0; v < this->height(); v++) {
            penColor = getColor(m_hue, s / this->width(), v / this->height());
            if (!penColor.isValid()) {
                continue;
            }
            backgroundImage.setPixelColor(int(s), this->height() - 1 - int(v), penColor);
        }
    }
    QPixmap pm = QPixmap::fromImage(backgroundImage);
    QSize size(this->width(), this->height());
    QBitmap mask(size);
    QPainter painterMask(&mask);
    painterMask.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painterMask.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painterMask.setBrush(QColor(0, 0, 0));
    painterMask.drawRoundedRect(0, 0, size.width(), size.height(), 5, 5);

    QPixmap image = pm.scaled(size);
    image.setMask(mask);
    painter.drawPixmap(this->rect(),image);

}

void ColorLabel::enterEvent(QEvent *e)
{
    if (!m_workToPick)
        return;

    m_lastCursor = this->cursor();
    QLabel::enterEvent(e);
}

void ColorLabel::leaveEvent(QEvent *e)
{
    if (!m_workToPick)
        return;

    QLabel::leaveEvent(e);
}

void ColorLabel::mousePressEvent(QMouseEvent *e)
{
    if (!m_workToPick)
        return ;

    m_pressed = true;
    QLabel::mousePressEvent(e);
}

void ColorLabel::mouseMoveEvent(QMouseEvent *e)
{
    if (!m_workToPick)
        return;

    pickColor(e->pos(), m_pressed);

    update();
    //移动事件不传递到父控件中
    e->accept();
}

void ColorLabel::mouseReleaseEvent(QMouseEvent *e)
{
    if (m_pressed && rect().contains(e->pos())) {
        m_clickedPos = e->pos();
        emit clicked();
    }

    m_pressed = false;
    QLabel::mouseReleaseEvent(e);
}

ColorLabel::~ColorLabel()
{
}

QCursor ColorLabel::pickColorCursor()
{
    int tipWidth = 11;
    QPixmap cursorPix = QPixmap(QSize(tipWidth, tipWidth));
    cursorPix.fill(QColor(Qt::transparent));

    QPen whitePen;
    whitePen.setWidth(1);
    whitePen.setCapStyle(Qt::FlatCap);
    whitePen.setJoinStyle(Qt::RoundJoin);
    whitePen.setColor(QColor(255, 255, 255, 255));

    QPen blackPen;
    blackPen.setWidth(1);
    blackPen.setCapStyle(Qt::FlatCap);
    blackPen.setJoinStyle(Qt::RoundJoin);
    blackPen.setColor(QColor(0, 0, 0, 125));

    QPainter painter(&cursorPix);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(blackPen);
    painter.drawEllipse(cursorPix.rect().center(), tipWidth / 2 - 1, tipWidth / 2 - 1);
    painter.drawEllipse(cursorPix.rect().center(), tipWidth / 2 - 3, tipWidth / 2 - 3);
    painter.setPen(whitePen);
    painter.drawEllipse(cursorPix.rect().center(), tipWidth / 2 - 2, tipWidth / 2 - 2);

    return QCursor(cursorPix, -1, -1);
}

