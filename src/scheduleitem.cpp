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
#include <QDebug>
#include <QFontMetricsF>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <DGraphicsView>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <dtkwidget_global.h>
#include "schedulecoormanage.h"
#include "scheduledatamanage.h"
#include "SchecduleRemindWidget.h"
#include <DFontSizeManager>

DWIDGET_USE_NAMESPACE

CScheduleItem::CScheduleItem(CScheduleCoorManage *coor, QGraphicsItem *parent,
                             QGraphicsScene *scene, int type)
    : QGraphicsItem(parent)
    , m_coorManage(coor)
    , m_type(type)
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    setFocus();
    setZValue(1);

    setFlag(QGraphicsItem::ItemIgnoresTransformations);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(ItemIsSelectable, true);
    m_transparentcolor = "#000000";
    m_transparentcolor.setAlphaF(0.05);
    scene->addItem(this);
    const int duration = 200;
    m_properAnimationFirst = new  QPropertyAnimation(this, "offset", this);
    m_properANimationSecond  = new QPropertyAnimation(this, "offset", this);
    m_properAnimationFirst->setDuration(duration);
    m_properANimationSecond->setDuration(duration);
    m_Group = new QSequentialAnimationGroup(this);
    m_Group->addAnimation(m_properAnimationFirst);
    m_Group->addAnimation(m_properANimationSecond);
//    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    // setAcceptHoverEvents(true);
    // setAcceptedMouseButtons(Qt::LeftButton);
    // setAcceptTouchEvents(true);
}

CScheduleItem::~CScheduleItem() {}

void CScheduleItem::setData(const ScheduleDtailInfo &info, QDate date, int index, int totalNum,
                            int viewtype, int maxnum)
{
    // if (m_type == 0) setToolTip(info.titleName);
    m_scheduleInfo = info;
    m_color = info.type.color;
    m_index = index;
    m_totalNum = totalNum;
    m_date = date;
    m_viewtype = viewtype;
    m_sMaxNum = maxnum;
    update();
}

bool CScheduleItem::hasSelectSchedule(const ScheduleDtailInfo &info)
{
    return (info.id == m_scheduleInfo.id) && (info.RecurID == m_scheduleInfo.RecurID);
}

QRectF CScheduleItem::boundingRect() const
{
    QRect t_rect;

    t_rect = m_coorManage->getDrawRegion(m_date, m_scheduleInfo.beginDateTime,
                                         m_scheduleInfo.endDateTime, m_index, m_totalNum, m_sMaxNum,
                                         m_viewtype);
    t_rect = QRect(t_rect.x() - m_offset / 2,
                   t_rect.y() - m_offset / 2,
                   t_rect.width() + m_offset,
                   t_rect.height() + m_offset);
    return t_rect;
}

QPainterPath CScheduleItem::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void CScheduleItem::updateitem()
{
    m_hoverflag = false;
    m_selectflag = false;
    emit signalsSelectUpdateState(this, 0);
    emit signalsHoverUpdateState(this, 0);
    update();
}

void CScheduleItem::UpdateSelectState(int state)
{
    if (state == 0) {
        m_selectflag = false;
    } else {
        m_selectflag = true;
    }
    update();
}

void CScheduleItem::setOffset(const int size)
{
    m_offset = size;
    update();
}

void CScheduleItem::setStartValue(const int value)
{
    m_properAnimationFirst->setStartValue(value);
    m_properANimationSecond->setEndValue(value);
}

void CScheduleItem::setEndValue(const int value)
{
    m_properAnimationFirst->setEndValue(value);
    m_properANimationSecond->setStartValue(value);
}

void CScheduleItem::startAnimation()
{
    if (m_Group->state() != QAnimationGroup::Running) {
        m_Group->start();
    }
}

void CScheduleItem::UpdateHoverState(int state)
{
    if (state == 0) {
        m_hoverflag = false;
    } else {
        m_hoverflag = true;
    }
    update();
}

void CScheduleItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_hoverflag = true;
    update();
    emit signalsHoverUpdateState(this, 1);
}

void CScheduleItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_hoverflag = false;
    m_hoverPressMove = false;
    update();
    emit signalsHoverUpdateState(this, 0);
}

void CScheduleItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_selectflag = true;
        m_hoverPressMove = true;
        update();
        emit signalsSelectUpdateState(this, 1);
    }
}

void CScheduleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_selectflag = false;
        m_hoverPressMove = false;
        update();
        emit signalsSelectUpdateState(this, 0);
    }
}

void CScheduleItem::focusOutEvent(QFocusEvent *event)
{
    // m_highflag = false;
    update();
}

void CScheduleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                          QWidget *widget /*= 0 */)
{
    CSchedulesColor gdcolor = CScheduleDataManage::getScheduleDataManage()->getScheduleColorByType(
                                  m_scheduleInfo.type.ID);
    m_highflag = CScheduleDataManage::getScheduleDataManage()->getSearchResult(m_scheduleInfo);
    int themetype = CScheduleDataManage::getScheduleDataManage()->getTheme();

    QColor bcolor = gdcolor.Purecolor;

    QFontMetrics fm = painter->fontMetrics();
    int h = fm.height();

    if (m_hoverflag) {
        bcolor = gdcolor.hoverPurecolor;
    } else if (m_highflag) {
        bcolor = gdcolor.hightlightPurecolor;
    } else if (m_selectflag) {
        bcolor = gdcolor.pressPurecolor;
    }
    painter->setBrush(bcolor);
    painter->setPen(Qt::NoPen);
    QRect rect = m_coorManage->getDrawRegion(m_date, m_scheduleInfo.beginDateTime,
                                             m_scheduleInfo.endDateTime, m_index, m_totalNum,
                                             m_sMaxNum, m_viewtype);
    rect = QRect(rect.x() - m_offset / 2,
                 rect.y() - m_offset / 2,
                 rect.width() + m_offset,
                 rect.height() + m_offset);
    painter->drawRect(rect);
    if (m_hoverflag && !m_selectflag) {
        painter->save();

        QRectF trect = QRectF(rect.x() + 0.5, rect.y() + 0.5, rect.width() - 1, rect.height() - 1);
        QPen tpen;
        QColor cc = "#FFFFFF";
        if (themetype == 2) {
            cc = "#FFFFFF";
        } else {
            cc = "#000000";
        }
        cc.setAlphaF(0.08);
        tpen.setColor(cc);
        tpen.setWidthF(1);
        tpen.setStyle(Qt::SolidLine);
        painter->setBrush(Qt::NoBrush);
        painter->setPen(tpen);
        painter->drawRect(trect);
        painter->restore();
    }
    if (m_selectflag) {
        gdcolor.textColor.setAlphaF(0.6);
        gdcolor.timeColor.setAlphaF(0.6);
    }
    painter->save();

    QPen pen(gdcolor.shadowcolor);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->drawLine(rect.topLeft(), rect.bottomLeft());
    painter->restore();
    int tmagin = 10;
    if (m_totalNum > 1)
        tmagin = 5;
    if (m_type == 0) {
        int timeTextHight = 0;
        QFont font("SourceHanSansSC");
        font.setWeight(QFont::Normal);
        // if (m_totalNum > 1) {
        // font.setPixelSize(6);
        //} else {
        font = DFontSizeManager::instance()->get(DFontSizeManager::T8,font);
//        font.setPixelSize(12);
        //}
        if (m_scheduleInfo.beginDateTime.date() == m_date) {
            painter->save();
            painter->setFont(font);
            painter->setPen(gdcolor.timeColor);
            QTime stime = m_scheduleInfo.beginDateTime.time();
            //            QString str = stime.toString("ap HH:mm");
            QString str = stime.toString("AP h:mm");

            QFontMetrics fontmetris(font);
            if (fm.width(str) > rect.width() - 5) {
                QString tstr;
                for (int i = 0; i < str.count(); i++) {
                    tstr.append(str.at(i));
                    int widthT = fm.width(tstr) + 5;
                    if (widthT >= rect.width()) {
                        if (i < 2) {
                            tstr.chop(1);
                        } else {
                            tstr.chop(2);
                        }
                        tstr = tstr + "...";
                        break;
                    }
                }
                painter->drawText(
                    QRect(rect.topLeft().x() + tmagin, rect.topLeft().y(), rect.width() - 5, h),
                    Qt::AlignLeft, tstr);

            } else {
                painter->drawText(
                    QRect(rect.topLeft().x() + tmagin, rect.topLeft().y(), rect.width() - 5, h),
                    Qt::AlignLeft, str);
            }

            painter->restore();

        } else {
            timeTextHight = -20;
            //            rect.setWidth(20);
            //            qDebug() << rect;
        }

        painter->save();
        font = DFontSizeManager::instance()->get(DFontSizeManager::T6,font);
//        font.setPixelSize(14);
        painter->setFont(font);
        painter->setPen(gdcolor.textColor);
        QStringList liststr;
        splitText(font, rect.width() - tmagin - 8, rect.height() - 20, m_scheduleInfo.titleName,
                  liststr);
        for (int i = 0; i < liststr.count(); i++) {
            painter->drawText(
                QRect(rect.topLeft().x() + tmagin, rect.topLeft().y() + 20 + timeTextHight + i * 20,
                      rect.width() - 2, h),
                Qt::AlignLeft, liststr.at(i));
        }
        painter->restore();
    } else {
        painter->save();
        QFont font("SourceHanSansSC");
        font.setWeight(QFont::Normal);
        font = DFontSizeManager::instance()->get(DFontSizeManager::T8,font);
//        font.setPixelSize(12);
        painter->setFont(font);
        painter->setPen(gdcolor.textColor);
        painter->drawText(rect, Qt::AlignCenter | Qt::AlignVCenter, "...");
        painter->restore();
    }
    if (m_selectflag) {
        QColor selcolor = m_transparentcolor;
        selcolor.setAlphaF(0.05);
        painter->setBrush(selcolor);
        painter->setPen(Qt::NoPen);
        painter->drawRect(rect);
    }
}

void CScheduleItem::splitText(QFont font, int w, int h, QString str, QStringList &liststr)
{
    if (str.isEmpty())
        return;
    QFontMetrics fontmetris(font);
    // int widthT = fontmetris.width(str);
    int heightT = fontmetris.height();
    // int singlecharw = widthT * 1.0 / str.count() + 1;
    // int rcharcount = w * 1.0 / singlecharw;
#if 0
    QStringList sslist = str.split("\n");
    QStringList tliststr;
    for (int j = 0; j < sslist.count(); j++) {
        QString currentstr = sslist.at(j);
        QString tstr;
        int tcount = 0;
        for (int i = 0; i < currentstr.count(); i++) {
            tstr.append(currentstr.at(i));
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
    }
    if (w < 12) {
        liststr.append("...");
    } else {
        QString tstr;
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
#endif

#if 1
    QString tstr;
    QStringList tliststr;
    int tcount = 0;
    for (int i = 0; i < str.count(); i++) {
        tstr.append(str.at(i));
        int widthT = fontmetris.width(tstr) + 5;
        if (widthT >= w) {
            tstr.chop(1);
            if (tstr.isEmpty())
                break;
            tliststr.append(tstr);
            tstr.clear();
            i--;
        }
    }
    tliststr.append(tstr);
    if (w < 30) {
        if (tliststr.isEmpty()) {
            liststr.append("...");
        } else {
            liststr.append(tliststr.at(0) + "...");
        }
    } else {
        for (int i = 0; i < tliststr.count(); i++) {
            if ((i + 1) * heightT <= h) {
                liststr.append(tliststr.at(i));
            } else {
                if (i == 0) {
                    break;
                    // liststr.append("...");
                } else {
                    tstr = liststr.at(i - 1);
                    tstr.chop(3);
                    liststr[i - 1] = tstr + "...";
                }
                break;
            }
        }
    }
#endif
}
