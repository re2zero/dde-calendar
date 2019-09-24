/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     kirigaya <kirigaya@mkacg.com>
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
#include "scheduleitem.h"
#include <QPainter>
#include "schedulecoormanage.h"
#include "scheduledatamanage.h"
#include <QFontMetricsF>
#include <QGraphicsScene>
CScheduleItem::CScheduleItem(CScheduleCoorManage *coor, QGraphicsItem *parent, QGraphicsScene *scene, int type)
    : QGraphicsItem(parent), m_coorManage(coor), m_type(type)
{
    scene->addItem(this);
    setZValue(1);
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
    setFlag(ItemIsSelectable, true);
}

CScheduleItem::~CScheduleItem()
{

}

void CScheduleItem::setData( const ScheduleDtailInfo &info, int index, int totalNum)
{
    if (m_type == 0) setToolTip(info.titleName);
    m_scheduleInfo = info;
    m_color = info.type.color;
    m_index = index;
    m_totalNum = totalNum;
    update();
}

QRectF CScheduleItem::boundingRect() const
{
    QRectF t_rect;

    t_rect = m_coorManage->getDrawRegion(m_scheduleInfo.beginDateTime, m_scheduleInfo.endDateTime, m_index, m_totalNum);

    return t_rect;
}

QPainterPath CScheduleItem::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void CScheduleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0 */)
{
    CSchedulesColor gdcolor = CScheduleDataManage::getScheduleDataManage()->getScheduleColorByType(m_scheduleInfo.type.ID);

    painter->setBrush(gdcolor.Purecolor);
    painter->setPen(Qt::NoPen);
    QRect rect = m_coorManage->getDrawRegion(m_scheduleInfo.beginDateTime, m_scheduleInfo.endDateTime, m_index, m_totalNum);
    painter->drawRect(rect);
    painter->save();
    QPen pen(gdcolor.shadowcolor);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->drawLine(rect.topLeft(), rect.bottomLeft());
    painter->restore();
    if (m_type == 0) {
        painter->save();
        QFont font("SourceHanSansSC-Normal");
        //if (m_totalNum > 1) {
        //font.setPixelSize(6);
        //} else {
        font.setPixelSize(12);
        //}

        painter->setFont(font);

        painter->setPen(gdcolor.timeColor);
        QTime stime = m_scheduleInfo.beginDateTime.time();
        QString str = stime.toString("ap HH:mm");
        QFontMetrics fontmetris(font);
        int amwidth = fontmetris.width(str);
        if (fontmetris.width(str) > rect.width() - 2) {
            QString tstr;
            for (int i = 0; i < str.count(); i++) {
                tstr.append(str.at(i));
                int widthT = fontmetris.width(tstr) + 5;
                if (widthT >= rect.width()) {
                    tstr.chop(3);
                    tstr = tstr + "...";
                    break;
                }
            }
            painter->drawText(QRect(rect.topLeft().x() + 10, rect.topLeft().y(), rect.width() - 12, 20), Qt::AlignLeft, tstr);

        } else {
            painter->drawText(QRect(rect.topLeft().x() + 10, rect.topLeft().y(), rect.width() - 12, 20), Qt::AlignLeft, str);

        }
        painter->restore();

        painter->save();
        font.setPixelSize(14);
        painter->setFont(font);
        painter->setPen(gdcolor.textColor);
        QStringList liststr;
        splitText(font, rect.width() - 15, rect.height() - 20, m_scheduleInfo.titleName, liststr);
        for (int i = 0; i < liststr.count(); i++) {
            painter->drawText(QRect(rect.topLeft().x() + 10, rect.topLeft().y() + 20 + i * 20, rect.width() - 2, 20), Qt::AlignLeft, liststr.at(i));
        }
        painter->restore();
    } else {
        painter->save();
        QFont font("SourceHanSansSC-Normal");
        font.setPixelSize(12);
        painter->setFont(font);
        painter->setPen(gdcolor.textColor);
        painter->drawText(rect, Qt::AlignCenter | Qt::AlignVCenter, "...");
        painter->restore();
    }
}

void CScheduleItem::splitText( QFont font, int w, int h, QString str, QStringList &liststr )
{
    if (str.isEmpty()) return;
    QFontMetrics fontmetris(font);
    //int widthT = fontmetris.width(str);
    int heightT = fontmetris.height();
    //int singlecharw = widthT * 1.0 / str.count() + 1;
    //int rcharcount = w * 1.0 / singlecharw;
    QString tstr;
    QStringList tliststr;
    int tcount = 0;
    for (int i = 0; i < str.count(); i++) {
        tstr.append(str.at(i));
        int widthT = fontmetris.width(tstr) + 5;
        if (widthT >= w) {
            tstr.chop(1);
            if (tstr.isEmpty()) break;
            tliststr.append(tstr);
            tstr.clear();
            i--;
        }
    }
    tliststr.append(tstr);

    for (int i = 0; i < tliststr.count(); i++) {
        if ((i + 1)*heightT <= h) {
            liststr.append(tliststr.at(i));
        } else {
            if (i == 0) {
                break;
                //liststr.append("...");
            } else {
                tstr = liststr.at(i - 1);
                tstr.chop(3);
                liststr[i - 1] = tstr + "...";
            }
            break;
        }
    }
}
