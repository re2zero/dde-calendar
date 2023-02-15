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
#include "cmonthschedulenumitem.h"

#include <QPainter>

CMonthScheduleNumItem::CMonthScheduleNumItem(QGraphicsItem *parent)
    : CFocusItem(parent)
    , m_num(0)
{
    setItemType(COTHER);
}

CMonthScheduleNumItem::~CMonthScheduleNumItem()
{
}

/**
 * @brief CMonthScheduleNumItem::setColor       设置背景色
 * @param color1
 * @param color2
 */
void CMonthScheduleNumItem::setColor(QColor color1, QColor color2)
{
    m_color1 = color1;
    m_color2 = color2;
}

/**
 * @brief CMonthScheduleNumItem::setText        这是字体颜色
 * @param tColor
 * @param font
 */
void CMonthScheduleNumItem::setText(QColor tColor, QFont font)
{
    m_textcolor = tColor;
    m_font = font;
}

/**
 * @brief CMonthScheduleNumItem::setSizeType        设置字体大小
 * @param sizeType
 */
void CMonthScheduleNumItem::setSizeType(DFontSizeManager::SizeType sizeType)
{
    m_SizeType = sizeType;
}

void CMonthScheduleNumItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    qreal labelwidth = this->rect().width();
    qreal labelheight = this->rect().height() - 6;
    qreal rectX = this->rect().x();
    qreal rectY = this->rect().y();
    //绘制背景
    m_font = DFontSizeManager::instance()->get(m_SizeType, m_font);
    //将直线开始点设为0，终点设为1，然后分段设置颜色
    QLinearGradient linearGradient(0, 0, labelwidth, 0);
    linearGradient.setColorAt(0, m_color1);
    linearGradient.setColorAt(1, m_color2);

    painter->setRenderHints(QPainter::Antialiasing);
    painter->setBrush(linearGradient);
    if (getItemFocus()) {
        QPen framePen;
        framePen.setWidth(2);
        framePen.setColor(getSystemActiveColor());
        painter->setPen(framePen);
    } else {
        painter->setPen(Qt::NoPen);
    }
    painter->drawRoundedRect(rect(), rect().height() / 3, rect().height() / 3);
    //绘制文字
    painter->setFont(m_font);
    painter->setPen(m_textcolor);
    QString str = QString(tr("%1 more")).arg(m_num) + "...";
    QFontMetrics fm = painter->fontMetrics();
    QString tStr;
    for (int i = 0; i < str.count(); i++) {
        tStr.append(str.at(i));
        int widthT = fm.width(tStr) + 5;
        if (widthT >= labelwidth) {
            tStr.chop(2);
            break;
        }
    }
    if (tStr != str) {
        tStr = tStr + "...";
    }
    painter->drawText(QRectF(rectX, rectY, labelwidth, labelheight + 4), Qt::AlignCenter, tStr);
}
