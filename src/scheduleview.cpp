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
#include "schceduledayview.h"
#include "scheduledatamanage.h"
#include <DPalette>
DGUI_USE_NAMESPACE
static int hourTextLMagin = 16;
static int hourTextTMagin = 48;

static int hourTextWidth = 50;
static int hourTextHeight = 20;
CScheduleView::CScheduleView(QWidget *parent)
    : QFrame(parent)
{
    DPalette palette(this->palette());
    palette.setColor(DPalette::Background, Qt::white);
    this->setPalette(palette);
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
}

void CScheduleView::setRange( QDate begin, QDate end )
{
    m_TotalDay = begin.daysTo(end) + 1;
    m_graphicsView->getCoorManage()->setDateRange(begin, end);
    m_beginDate = begin;
    m_endDate = end;
    updateSchedule();
}

void CScheduleView::setFirstWeekday(int weekday)
{
    m_firstWeekDay = weekday;
    m_graphicsView->setFirstWeekday(weekday);
}

void CScheduleView::slotsupdatescheduleD(QWidget *w, QVector<ScheduleDateRangeInfo> &data)
{
    if (w != this) return;
    for (int i = 0; i < m_TotalDay; i++) {
        for (int j = 0; j < data.size(); j++) {
            if (data.at(j).date == m_beginDate.addDays(i)) {
                QVector<ScheduleDtailInfo> scheduleInfolist = data.at(j).vData;
                for (int  k = 0; k < scheduleInfolist.count(); k++) {
                    if (scheduleInfolist.at(k).allday) {
                        continue;
                    }
                    m_graphicsView->addSchduleItem(scheduleInfolist.at(k));
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

    painter.setPen(Qt::SolidLine);
    painter.setPen(Qt::lightGray);
    for (int i = 0; i < m_vPos.size(); i++) {
        if (m_vHours[i] == 0) continue;
        if (m_vHours[i] > 12) {
            painter.drawText(QRect(hourTextLMagin, m_topMagin - 8 + m_vPos[i], hourTextWidth, hourTextHeight), Qt::AlignCenter, tr("PM ") + QString::number(m_vHours[i] - 12) + tr(" h"));
        } else {
            painter.drawText(QRect(hourTextLMagin, m_topMagin - 8 + m_vPos[i], hourTextWidth, hourTextHeight), Qt::AlignCenter, tr("AM ") + QString::number(m_vHours[i]) + tr(" h"));
        }
    }
    painter.restore();
    painter.save();

    font.setPixelSize(14);
    painter.setPen(QColor("#303030"));
    painter.drawText(QRect(0, 0, m_leftMagin - 2, m_topMagin - 2), Qt::AlignCenter, tr("ALL DAY"));
    painter.restore();

    painter.setCompositionMode(QPainter::CompositionMode_Difference  ); //设置混合模式
    int t_width = width();
    int t_height = height();
    painter.save();
    painter.setPen(Qt::SolidLine);
    painter.setPen(Qt::lightGray);
    painter.drawLine(QPoint(0, m_topMagin), QPoint(t_width, m_topMagin));
    painter.restore();
    int intenval = (t_width - m_leftMagin) / m_TotalDay;
    if (m_TotalDay > 1) {
        painter.save();
        for (int i = 0; i < m_TotalDay; i++) {
            painter.drawLine(QPoint(m_leftMagin + i * intenval + 1, 1), QPoint(m_leftMagin  + i * intenval + 1, m_topMagin));
        }
        painter.restore();
        painter.save();
        for (int i = 0; i != 7; ++i) {

            int d = checkDay(i - m_firstWeekDay);
            QColor color("#E6EEF2");
            color.setAlphaF(0.05);
            painter.setBrush(color);
            painter.setPen(Qt::NoPen);
            if (d == 6 || d == 7) {
                painter.drawRect(QRect(m_leftMagin + i * intenval + 1, 0, intenval, m_topMagin));
            }
        }
        painter.restore();
    }
    painter.end();
}

void CScheduleView::resizeEvent(QResizeEvent *event)
{
    m_graphicsView->setRange(width() - 74, 24 * 43, m_beginDate, m_endDate);
    if (width() > 600)
        m_alldaylist->setFixedSize(635, 38);
    else {
        m_alldaylist->setFixedSize(width() - 90, 38);
    }
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
    m_alldaylist = new CSchceduleDayView(this);
    m_alldaylist->setFixedSize(635, 38);
    m_alldaylist->move(90, 10);
}

void CScheduleView::initConnection()
{
    connect(m_graphicsView, &CGraphicsView::signalsUpdateShcedule, this, &CScheduleView::slotupdateSchedule);
    //connect(m_graphicsView, &CGraphicsView::signalsUpdateShcedule, this, &CScheduleView::signalsUpdateShcedule);
    connect(m_alldaylist, &CSchceduleDayView::signalsUpdateShcedule, this, &CScheduleView::slotupdateSchedule);
    //connect(m_alldaylist, &CSchceduleDayView::signalsUpdateShcedule, this, &CScheduleView::signalsUpdateShcedule);
    connect(m_alldaylist, &CSchceduleDayView::signalsCotrlUpdateShcedule, this, &CScheduleView::slotCtrlSchceduleUpdate);

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
}

int CScheduleView::checkDay(int weekday)
{
    if (weekday <= 0)
        return weekday += 7;

    if (weekday > 7)
        return weekday -= 7;

    return weekday;
}

