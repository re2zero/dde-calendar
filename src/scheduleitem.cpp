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
CScheduleItem::CScheduleItem(CScheduleCoorManage *coor, QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsItem(parent), m_coorManage(coor)
{
    scene->addItem(this);
    setZValue(1);
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
    setFlag(ItemIsSelectable, true);
}

CScheduleItem::~CScheduleItem()
{

}

void CScheduleItem::setData( const ScheduleDtailInfo &info )
{
    m_scheduleInfo = info;
    m_color = info.type.color;
    update();
}

QRectF CScheduleItem::boundingRect() const
{
    QRectF t_rect;

    t_rect = m_coorManage->getDrawRegion(m_scheduleInfo.beginDateTime, m_scheduleInfo.endDateTime);

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
    QRect rect = m_coorManage->getDrawRegion(m_scheduleInfo.beginDateTime, m_scheduleInfo.endDateTime);
    painter->drawRect(rect);
    painter->save();
    QPen pen(gdcolor.shadowcolor);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->drawLine(rect.topLeft(), rect.bottomLeft());
    painter->restore();

    painter->save();
    QFont font("SourceHanSansSC-Normal");
    font.setPixelSize(12);
    painter->setFont(font);
    painter->setPen(gdcolor.timeColor);
    QTime stime = m_scheduleInfo.beginDateTime.time();
    QString str = stime.toString("ap HH:mm");
    painter->drawText(QRect(rect.topLeft().x() + 2, rect.topLeft().y(), rect.width() - 2, 20), Qt::AlignLeft, str);
    painter->restore();

    painter->save();
    font.setPixelSize(14);
    painter->setFont(font);
    painter->setPen(gdcolor.textColor);
    QStringList liststr;
    splitText(font, rect.width(), m_scheduleInfo.titleName, liststr);
    for (int i = 0; i < liststr.count(); i++) {
        painter->drawText(QRect(rect.topLeft().x() + 2, rect.topLeft().y() + 20 + i * 20, rect.width() - 2, 20), Qt::AlignLeft, liststr.at(i));
    }
    painter->restore();
}

void CScheduleItem::splitText( QFont font, int w, QString str, QStringList &liststr )
{
    if (str.isEmpty()) return;
    QFontMetrics fontmetris(font);
    int widthT = fontmetris.width(str);
    int singlecharw = widthT * 1.0 / str.count() + 0.5;
    int rcharcount = w * 1.0 / singlecharw;
    QString tstr;
    int tcount = 0;
    for (int i = 0; i < str.count(); i++) {
        if (tcount == rcharcount) {
            liststr.append(tstr);
            tstr.clear();
            tstr.append(str.at(i));
            tcount = 1;
        }
        tstr.append(str.at(i));
        tcount++;
    }
    liststr.append(tstr);
}
