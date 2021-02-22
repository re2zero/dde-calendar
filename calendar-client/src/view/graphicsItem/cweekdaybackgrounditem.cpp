/*
   * Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
   *
   * Author:     chenhaifeng <chenhaifeng@uniontech.com>
   *
   * Maintainer: chenhaifeng <chenhaifeng@uniontech.com>
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
#include "cweekdaybackgrounditem.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QMarginsF>

CWeekDayBackgroundItem::CWeekDayBackgroundItem(QGraphicsItem *parent)
    : CSceneBackgroundItem(parent)
    , m_drawDividingLine(false)
    , m_showFocus(false)
{
}

void CWeekDayBackgroundItem::setTheMe(int type)
{
    if (type == 0 || type == 1) {
        m_weekColor = "#00429A";
        m_weekColor.setAlphaF(0.05);
    } else {
        m_weekColor = "#4F9BFF";
        m_weekColor.setAlphaF(0.1);
    }
}

void CWeekDayBackgroundItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setRenderHint(QPainter::Antialiasing);
    if (m_drawDividingLine) {
        //绘制分割线
        if (this->getBackgroundNum() != 6) {
            // 分割线颜色
            QColor m_linecolor = "#000000";
            m_linecolor.setAlphaF(0.05);
            painter->setPen(Qt::SolidLine);
            painter->setPen(m_linecolor);
            painter->drawLine(this->rect().topRight(), this->rect().bottomRight());
        }
        //绘制周六周日背景
        if (m_Date.dayOfWeek() > 5) {
            painter->setBrush(m_weekColor);
            painter->setPen(Qt::NoPen);
            painter->drawRect(this->rect());
        }
    }
    if (m_showFocus && getItemFoucs()) {
        QPen framePen;
        //设置边框宽度
        framePen.setWidth(2);
        //设置边框颜色
        framePen.setColor(getSystemActiveColor());
        painter->setPen(framePen);
        painter->setBrush(Qt::NoBrush);
        QRectF drawrect = this->rect().marginsRemoved(QMarginsF(1, 1, 1, 1));
        painter->drawRect(drawrect);
    }
}

bool CWeekDayBackgroundItem::showFocus() const
{
    return m_showFocus;
}

void CWeekDayBackgroundItem::setShowFocus(bool showFocus)
{
    m_showFocus = showFocus;
}

bool CWeekDayBackgroundItem::drawDividingLine() const
{
    return m_drawDividingLine;
}

void CWeekDayBackgroundItem::setDrawDividingLine(bool drawDividingLine)
{
    m_drawDividingLine = drawDividingLine;
}
