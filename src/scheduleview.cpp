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
#include "scheduleview.h"
#include <QGridLayout>
#include "graphicsview.h"
#include "schedulecoormanage.h"
#include "dbmanager.h"
#include "alldayschceduleview.h"
#include "scheduledatamanage.h"
#include <DPalette>
DGUI_USE_NAMESPACE
static int hourTextLMagin = 16;
static int hourTextTMagin = 48;

static int hourTextWidth = 50;
static int hourTextHeight = 20;
CScheduleView::CScheduleView(QWidget *parent, int viewType)
    : DFrame(parent), m_viewType(viewType)
{

    setAutoFillBackground(true);
    initUI();
    initConnection();
}

CScheduleView::~CScheduleView()
{

}

void CScheduleView::setviewMagin( int left, int top, int right, int bttom )
{
    m_leftMagin = left;
    m_topMagin = top;
    m_graphicsView->setMargins(left, top, right, bttom);
}

void CScheduleView::setRange( int w, int h, QDate begin, QDate end )
{
    m_TotalDay = begin.daysTo(end) + 1;
    m_graphicsView->setRange(w, h, begin, end);
    m_alldaylist->setRange(w, 22, m_beginDate, m_endDate);
}

void CScheduleView::setRange( QDate begin, QDate end )
{
    m_TotalDay = begin.daysTo(end) + 1;
    m_graphicsView->getCoorManage()->setDateRange(begin, end);
    m_alldaylist->getCoorManage()->setDateRange(begin, end);
    m_beginDate = begin;
    m_endDate = end;
    updateSchedule();
}

void CScheduleView::setFirstWeekday(int weekday)
{
    m_firstWeekDay = weekday;
    m_graphicsView->setFirstWeekday(weekday);
}

void CScheduleView::setTheMe(int type)
{
    if (type == 0 || type == 1) {
        DPalette palette(this->palette());
        palette.setColor(DPalette::Background, "#FFFFFF");
        this->setPalette(palette);
        m_linecolor = "#000000";
        m_linecolor.setAlphaF(0.1);
        m_weekColor = "#00429A";
        m_weekColor.setAlphaF(0.05);
        m_ALLDayColor = "#303030";
        m_timeColor = "#7D7D7D";

    } else if (type == 2) {
        DPalette palette(this->palette());
        QColor tbcolor = "#FFFFFF";
        tbcolor.setAlphaF(0.05);
        palette.setColor(DPalette::Background, tbcolor);
        this->setPalette(palette);
        m_linecolor = "#000000";
        m_linecolor.setAlphaF(0.1);
        m_weekColor = "#4F9BFF";
        m_weekColor.setAlphaF(0.1);
        m_ALLDayColor = "#7D7D7D";
        m_timeColor = "#7D7D7D";
    }
    m_graphicsView->setTheMe(type);
    m_alldaylist->setTheMe(type);
    update();
}

void CScheduleView::setLunarVisible(bool state)
{
    m_alldaylist->setLunarVisible(state);
}
bool MScheduleTimeThan(const ScheduleDtailInfo &s1, const ScheduleDtailInfo &s2)
{
    if (s1.beginDateTime.secsTo(s1.endDateTime) == s2.beginDateTime.secsTo(s2.endDateTime)) {
        return  s1.beginDateTime < s2.beginDateTime;
    } else {
        return  s1.beginDateTime.secsTo(s1.endDateTime) > s2.beginDateTime.secsTo(s2.endDateTime);
    }
}
void CScheduleView::scheduleClassificationType(QVector<ScheduleDtailInfo> &scheduleInfolist, QVector<ScheduleclassificationInfo> &info)
{
    QVector<ScheduleDtailInfo> schedulelist;
    for (int  k = 0; k < scheduleInfolist.count(); k++) {
        if (scheduleInfolist.at(k).allday) {
            continue;
        }
        schedulelist.append(scheduleInfolist.at(k));
    }
    if (schedulelist.isEmpty()) return;

    info.clear();
    qSort(schedulelist.begin(), schedulelist.end(), MScheduleTimeThan);
    for (int  k = 0; k < schedulelist.count(); k++) {
        int i = 0;
        for (; i < info.count(); i++) {
            if ((schedulelist.at(k).beginDateTime >= info.at(i).begindate && schedulelist.at(k).beginDateTime <= info.at(i).enddate)
                    || (schedulelist.at(k).endDateTime >= info.at(i).begindate && schedulelist.at(k).endDateTime <= info.at(i).enddate)
                    || (schedulelist.at(k).beginDateTime >= info.at(i).begindate && schedulelist.at(k).endDateTime <= info.at(i).enddate)
                    || (schedulelist.at(k).beginDateTime <= info.at(i).begindate && schedulelist.at(k).endDateTime >= info.at(i).enddate) ) {

                break;
            }
        }
        if (i == info.count()) {
            ScheduleclassificationInfo firstschedule;
            firstschedule.begindate  = schedulelist.at(k).beginDateTime;
            firstschedule.enddate  = schedulelist.at(k).endDateTime;
            firstschedule.vData.append(schedulelist.at(k));
            info.append(firstschedule);
        } else {
            if (schedulelist.at(k).beginDateTime < info.at(i).begindate) {
                info[i].begindate = schedulelist.at(k).beginDateTime;
            }
            if (schedulelist.at(k).endDateTime > info.at(i).enddate) {
                info[i].enddate = schedulelist.at(k).endDateTime;
            }
            info[i].vData.append(schedulelist.at(k));
        }
    }

}
void CScheduleView::slotsupdatescheduleD(QWidget *w, QVector<ScheduleDateRangeInfo> &data)
{
    if (w != this) return;
    m_graphicsView->clearSchdule();
    for (int i = 0; i < m_TotalDay; i++) {
        for (int j = 0; j < data.size(); j++) {
            if (data.at(j).date == m_beginDate.addDays(i)) {
                QVector<ScheduleDtailInfo> scheduleInfolist = data.at(j).vData;

                QVector<ScheduleclassificationInfo> info;
                scheduleClassificationType(scheduleInfolist, info);

                QDate tdate = m_beginDate.addDays(i);
                for (int m = 0; m  < info.count(); m++) {
                    int tnum = info.at(m).vData.count();
                    if (m_viewType == 0) {
                        if (tnum > 3) {
                            tnum = 4;
                            for (int n = 0; n  < tnum - 1; n++) {
                                m_graphicsView->addSchduleItem(info.at(m).vData.at(n), tdate, n + 1, tnum, 0, m_viewType);
                            }
                            ScheduleDtailInfo tdetaliinfo = info.at(m).vData.at(1);
                            tdetaliinfo.titleName = "...";
                            tdetaliinfo.type.ID = 3;
                            m_graphicsView->addSchduleItem(tdetaliinfo, tdate, 4, tnum, 1, m_viewType);
                        } else {
                            for (int n = 0; n  < tnum; n++) {
                                m_graphicsView->addSchduleItem(info.at(m).vData.at(n), tdate, n + 1, tnum, 0, m_viewType);
                            }
                        }

                    } else {
                        for (int n = 0; n  < tnum; n++) {
                            m_graphicsView->addSchduleItem(info.at(m).vData.at(n), tdate, n + 1, tnum, 0, m_viewType);
                        }
                    }
                }
                break;
            }
        }
    }
    setEnabled(true);
}

void CScheduleView::setDate( QDate date )
{
    m_currteDate = date;
    m_alldaylist->setsolarDayData(QString());
    updateAllday();
}

void CScheduleView::setDate(QDate date, QString solarDay)
{
    m_currteDate = date;
    m_alldaylist->setsolarDayData(solarDay);
    updateAllday();
}

void CScheduleView::slotupdateSchedule(int id)
{
    updateSchedule(id);
    updateAllday(id);
}

void CScheduleView::slotPosHours(QVector<int> vPos, QVector<int> vHours)
{
    m_vHours = vHours;
    m_vPos = vPos;
    update();
}

void CScheduleView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QFont font;
    font.setFamily("SourceHanSansSC-Normal");
    font.setPixelSize(11);
    painter.save();
    painter.setFont(font);

    //painter.setPen(Qt::SolidLine);
    painter.setPen(m_timeColor);
    for (int i = 0; i < m_vPos.size(); i++) {
        if (m_vHours[i] == 0) continue;
        if (m_vHours[i] > 12) {
            painter.drawText(QRect((m_leftMagin - hourTextWidth) / 2 - 5, m_topMagin - 8 + m_vPos[i], hourTextWidth, hourTextHeight), Qt::AlignRight, tr("PM ") + QString::number(m_vHours[i] - 12) + tr(" h"));
        } else {
            painter.drawText(QRect((m_leftMagin - hourTextWidth) / 2 - 5, m_topMagin - 8 + m_vPos[i], hourTextWidth, hourTextHeight), Qt::AlignRight, tr("AM ") + QString::number(m_vHours[i]) + tr(" h"));
        }
    }
    painter.restore();
    painter.save();
    QFont alldayfont;
    alldayfont.setFamily("SourceHanSansSC-Medium");
    alldayfont.setPixelSize(14);
    painter.setFont(alldayfont);
    painter.setPen(m_ALLDayColor);
    painter.drawText(QRect(0, 0, m_leftMagin - 2, m_topMagin - 2), Qt::AlignCenter, tr("ALL DAY"));
    painter.restore();

    //painter.setCompositionMode(QPainter::CompositionMode_Difference  ); //设置混合模式
    int t_width = width();
    int t_height = height();
    painter.save();
    painter.setPen(Qt::SolidLine);
    painter.setPen(m_linecolor);
    painter.drawLine(QPoint(0, m_topMagin), QPoint(t_width, m_topMagin));
    painter.restore();
    float intenval = 1.0 * (t_width - m_leftMagin) / m_TotalDay;
    if (m_TotalDay > 1) {
        painter.save();
        painter.setPen(Qt::SolidLine);
        painter.setPen(m_linecolor);
        for (int i = 1; i < m_TotalDay; i++) {
            painter.drawLine(QPoint(m_leftMagin + i * intenval, 1), QPoint(m_leftMagin  + i * intenval, m_topMagin));
        }
        painter.restore();
        painter.save();
        for (int i = 0; i != 7; ++i) {

            int d = checkDay(i - m_firstWeekDay);
            painter.setBrush(m_weekColor);
            painter.setPen(Qt::NoPen);
            if (d == 6 ) {
                painter.drawRect(QRect(m_leftMagin + i * intenval, 0, intenval, m_topMagin));
            }
            if (d == 7) {
                painter.drawRect(QRect(m_leftMagin + i * intenval + 1, 0, intenval, m_topMagin));
            }
        }
        painter.restore();
    }
    painter.end();
}

void CScheduleView::resizeEvent(QResizeEvent *event)
{
    m_graphicsView->resize(width(), height());
    int tt = 0.0968 * height() + 0.5;
    if (m_viewType == 0) {
        m_graphicsView->setRange(width() - m_leftMagin, 24 * (0.0968 * height() + 0.5), m_beginDate, m_endDate);
        m_alldaylist->setFixedSize(width() - m_leftMagin, m_topMagin - 10);
        m_alldaylist->setRange(width() - m_leftMagin, 22, m_beginDate, m_endDate);
        m_alldaylist->move(m_leftMagin - 2, 5);
    } else {
        m_graphicsView->setRange(width() - m_leftMagin, 24 * (0.083 * height() + 0.5), m_beginDate, m_endDate);
        m_alldaylist->setFixedSize(width() - m_leftMagin, m_topMagin - 10);
        m_alldaylist->setRange(width() - m_leftMagin, 22, m_beginDate, m_endDate);
        m_alldaylist->move(m_leftMagin - 2, 5);
    }
    update();
    m_graphicsView->update();
    m_alldaylist->update();
    QFrame::resizeEvent(event);
}

void CScheduleView::initUI()
{
    QGridLayout *layout = new QGridLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    m_graphicsView = new CGraphicsView(0);
    connect(m_graphicsView, SIGNAL(signalsPosHours(QVector<int>, QVector<int> )), this, SLOT(slotPosHours(QVector<int>, QVector<int> )));
    layout->addWidget(m_graphicsView);
    setLayout(layout);
    m_graphicsView->scrollBarValueChangedSlot();
    m_alldaylist = new CAllDaySchceduleView(this, 1);
    //m_alldaylist->setFixedSize(635, 99);
    m_alldaylist->move(72, 5);
}

void CScheduleView::initConnection()
{
    connect(m_graphicsView, &CGraphicsView::signalsUpdateShcedule, this, &CScheduleView::slotupdateSchedule);
    //connect(m_graphicsView, &CGraphicsView::signalsUpdateShcedule, this, &CScheduleView::signalsUpdateShcedule);
    connect(m_alldaylist, &CAllDaySchceduleView::signalsUpdateShcedule, this, &CScheduleView::slotupdateSchedule);
    //connect(m_alldaylist, &CSchceduleAllDayView::signalsUpdateShcedule, this, &CScheduleView::signalsUpdateShcedule);
    connect(m_alldaylist, &CAllDaySchceduleView::signalsCotrlUpdateShcedule, this, &CScheduleView::slotCtrlSchceduleUpdate);

    CScheduleDataCtrl  *scheduleDataCtrl = CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl();
    connect(scheduleDataCtrl, &CScheduleDataCtrl::signalsupdatescheduleD, this, &CScheduleView::slotsupdatescheduleD);
    connect(this, &CScheduleView::signalsupdatescheduleD, scheduleDataCtrl, &CScheduleDataCtrl::slotupdatescheduleD);
}
void CScheduleView::slotCtrlSchceduleUpdate(QDate date, int type)
{
    updateSchedule(0);
    updateAllday(0);
}
void CScheduleView::updateSchedule(int id)
{
    m_graphicsView->clearSchdule();
    setEnabled(false);
    emit signalsupdatescheduleD(this, m_beginDate, m_endDate);
}

void CScheduleView::updateAllday(int id)
{
    QVector<ScheduleDateRangeInfo> out;
    CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfo(m_currteDate, m_currteDate, out);
    QVector<ScheduleDtailInfo> vData;
    if (!out.isEmpty())  {
        for (int i = 0; i < out[0].vData.size(); i++) {
            if (out[0].vData[i].allday)
                vData.append(out[0].vData[i]);
        }
    }
    m_alldaylist->setDayData(m_currteDate, vData, 0);
    // m_alldaylist->update();
}

int CScheduleView::checkDay(int weekday)
{
    if (weekday <= 0)
        return weekday += 7;

    if (weekday > 7)
        return weekday -= 7;

    return weekday;
}

