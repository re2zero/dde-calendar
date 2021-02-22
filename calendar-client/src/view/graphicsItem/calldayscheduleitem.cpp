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
#include "calldayscheduleitem.h"

#include <QPainter>

CAllDayScheduleItem::CAllDayScheduleItem(QRectF rect, QGraphicsItem *parent)
    : DragInfoItem(rect, parent)
{
}

bool CAllDayScheduleItem::hasSelectSchedule(const ScheduleDataInfo &info)
{
    return info == m_vScheduleInfo;
}

void CAllDayScheduleItem::paintBackground(QPainter *painter, const QRectF &rect, const int isPixMap)
{
    Q_UNUSED(isPixMap);
    m_font = DFontSizeManager::instance()->get(m_sizeType, m_font);
    painter->setRenderHints(QPainter::Antialiasing);
    CSchedulesColor gdcolor = CScheduleDataManage::getScheduleDataManage()->getScheduleColorByType(m_vScheduleInfo.getType());
    QRectF drawrect = rect;
    QLinearGradient linearGradient(drawrect.topLeft().x(), 0, drawrect.topRight().x(), 0);

    QColor color1 = gdcolor.gradientFromC;
    QColor color2 = gdcolor.gradientToC;
    QColor textcolor = gdcolor.textColor;

    //判断是否为选中日程
    if (m_vScheduleInfo == m_pressInfo) {
        //判断当前日程是否为拖拽移动日程
        if (m_vScheduleInfo.getIsMoveInfo() == m_pressInfo.getIsMoveInfo()) {
            m_vHighflag = true;
        } else {
            painter->setOpacity(0.4);
            textcolor.setAlphaF(0.4);
        }
        m_vSelectflag = m_press;
    }
    int themetype = CScheduleDataManage::getScheduleDataManage()->getTheme();

    if (m_vHoverflag) {
        color1 = gdcolor.hovergradientFromC;
        color2 = gdcolor.hovergradientToC;
    } else if (m_vHighflag) {
        color1 = gdcolor.hightlightgradientFromC;
        color2 = gdcolor.hightlightgradientToC;
    } else if (m_vSelectflag) {
        color1 = gdcolor.pressgradientFromC;
        color2 = gdcolor.pressgradientToC;
        textcolor.setAlphaF(0.4);
    }
    linearGradient.setColorAt(0, color1);
    linearGradient.setColorAt(1, color2);
    QRectF fillRect = QRectF(drawrect.x(),
                             drawrect.y(),
                             drawrect.width(),
                             drawrect.height() - 2);
    //将直线开始点设为0，终点设为1，然后分段设置颜色
    painter->setBrush(linearGradient);
    if (getItemFoucs() && isPixMap == false) {
        QPen framePen;
        framePen.setWidth(2);
        framePen.setColor(getSystemActiveColor());
        painter->setPen(framePen);
    } else {
        painter->setPen(Qt::NoPen);
    }
    painter->drawRoundedRect(fillRect, rect.height() / 3, rect.height() / 3);
    painter->setFont(m_font);
    painter->setPen(textcolor);
    QFontMetrics fm = painter->fontMetrics();
    QString tStitlename = m_vScheduleInfo.getTitleName();
    tStitlename.replace("\n", "");
    QString str = tStitlename;
    QString tstr;
    int _rightOffset = fm.width("...");
    //显示宽度  左侧偏移13右侧偏移8
    qreal _showWidth = fillRect.width() - 13 - 8 - m_offset * 2;
    //如果标题总长度大于显示长度则显示长度须减去"..."的长度
    if (fm.width(str) > _showWidth) {
        _showWidth -= _rightOffset;
        for (int i = 0; i < str.count(); i++) {
            tstr.append(str.at(i));
            int widthT = fm.width(tstr);
            //如果宽度大于显示长度则去除最后添加的字符
            if (widthT > _showWidth) {
                tstr.chop(1);
                break;
            }
        }
        if (tstr != str) {
            tstr = tstr + "...";
        }
    } else {
        tstr = str;
    }

    painter->drawText(QRectF(fillRect.topLeft().x() + 13, fillRect.y(), fillRect.width(), fillRect.height()),
                      Qt::AlignLeft | Qt::AlignVCenter, tstr);
    if (m_vHoverflag && !m_vSelectflag) {
        QRectF trect = QRectF(fillRect.x() + 0.5, fillRect.y() + 0.5, fillRect.width() - 1, fillRect.height() - 1);
        painter->save();

        QPen pen;
        QColor selcolor;

        if (themetype == 2) {
            selcolor = "#FFFFFF";
        } else {
            selcolor = "#000000";
        }
        selcolor.setAlphaF(0.08);
        pen.setColor(selcolor);
        pen.setWidthF(1);
        pen.setStyle(Qt::SolidLine);
        painter->setBrush(Qt::NoBrush);
        painter->setPen(pen);
        painter->drawRoundedRect(trect, rect.height() / 3, rect.height() / 3);
        painter->restore();
    }
    if (m_vSelectflag) {
        QColor selcolor = "#000000";
        selcolor.setAlphaF(0.05);
        painter->setBrush(selcolor);
        painter->setPen(Qt::NoPen);
        painter->drawRoundedRect(fillRect, rect.height() / 3, rect.height() / 3);
    }
}
