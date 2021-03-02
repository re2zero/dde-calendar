/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
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

#include "monthview.h"
#include "scheduledlg.h"
#include "scheduledatamanage.h"

#include <DHiDPIHelper>
#include <DPalette>

#include <QPainter>
#include <QEvent>
#include <QTime>
#include <QApplication>
#include <QMouseEvent>

DGUI_USE_NAMESPACE

/**
 * @brief setTheMe  根据系统主题类型设置颜色
 * @param type      系统主题类型
 */
void CMonthView::setTheMe(int type)
{
    m_weekIndicator->setTheMe(type);
    m_monthGraphicsView->setTheMe(type);
}

CMonthView::CMonthView(QWidget *parent) : DWidget(parent)
{
    m_weekIndicator = new CMonthWeekView;
    m_monthGraphicsView = new CMonthGraphiview(this);

    connect(m_monthGraphicsView, &CMonthGraphiview::signalsViewSelectDate, this, &CMonthView::signalsViewSelectDate);
    connect(m_monthGraphicsView, &CMonthGraphiview::signalViewtransparentFrame, this, &CMonthView::signalViewtransparentFrame);
    connect(m_monthGraphicsView, &CMonthGraphiview::signalScheduleShow, this, &CMonthView::slotScheduleRemindWidget);
    connect(m_monthGraphicsView, &CMonthGraphiview::signalAngleDelta, this, &CMonthView::signalAngleDelta);
    connect(m_monthGraphicsView, &CMonthGraphiview::signalSwitchPrePage, this, &CMonthView::signalSwitchPrePage);
    connect(m_monthGraphicsView, &CMonthGraphiview::signalSwitchNextPage, this, &CMonthView::signalSwitchNextPage);
    connect(m_monthGraphicsView, &CMonthGraphiview::signalGotoDayView, this, &CMonthView::signalsViewSelectDate);
    //新建最终布局
    m_mainLayout = new QVBoxLayout;
    m_mainLayout->setMargin(0);
    m_mainLayout->setSpacing(0);
    m_mainLayout->setContentsMargins(10, 0, 0, 10);
    m_mainLayout->addWidget(m_weekIndicator);
    m_mainLayout->addWidget(m_monthGraphicsView);

    setLayout(m_mainLayout);
    m_createAction = new QAction(tr("New event"), this);

    m_remindWidget = new SchecduleRemindWidget(this);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

CMonthView::~CMonthView()
{
}

void CMonthView::setSelectSchedule(const ScheduleDataInfo &scheduleInfo)
{
    m_monthGraphicsView->setSelectSearchSchedule(scheduleInfo);
}

void CMonthView::slotScheduleRemindWidget(const bool isShow, const ScheduleDataInfo &out)
{
    if (isShow) {
        //获取当前鼠标位置
        QPoint remindPos = QCursor::pos();
        CSchedulesColor gdcolor = CScheduleDataManage::getScheduleDataManage()->getScheduleColorByType(
                                      out.getType());
        m_remindWidget->setData(out, gdcolor);
        //获取屏幕大小
        QRect desktopRect = QApplication::desktop()->rect();
        //根据提示框在屏幕的位置设置箭头方向
        if ((remindPos.x() + m_remindWidget->width() + 10) > desktopRect.width()) {
            m_remindWidget->setDirection(DArrowRectangle::ArrowRight);
            remindPos.setX(remindPos.x() - 5);
        } else {
            m_remindWidget->setDirection(DArrowRectangle::ArrowLeft);
            remindPos.setX(remindPos.x() + 5);
        }
        m_remindWidget->show(remindPos.x(), remindPos.y());
    } else {
        m_remindWidget->hide();
    }
}

void CMonthView::resizeEvent(QResizeEvent *event)
{
    DWidget::resizeEvent(event);
    int leftmagin = 10;
    int topmagin = 10;
    m_leftmaagin = leftmagin;
    m_topmagin = topmagin;
    m_mainLayout->setContentsMargins(leftmagin, topmagin, 0, 10);
    m_weekIndicator->setFixedSize(width() - leftmagin, static_cast<int>(height() * 0.1042 + 0.5));
}

void CMonthView::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    slotScheduleRemindWidget(false);
}

bool CMonthView::event(QEvent *event)
{
    if (event->type() == QEvent::FocusIn) {
        m_monthGraphicsView->setFocus(Qt::TabFocusReason);
        return true;
    }
    return DWidget::event(event);
}

/**
 * @brief setFirstWeekday   设置每周的第一天是周几
 * @param weekday           周几
 */
void CMonthView::setFirstWeekday(Qt::DayOfWeek weekday)
{
    m_firstWeekDay = weekday;
    m_weekIndicator->setFirstDay(weekday);
}

/**
 * @brief CMonthView::setShowDate       设置显示日期
 * @param showDate
 */
void CMonthView::setShowDate(const QVector<QDate> &showDate)
{
    m_showDate = showDate;
    m_monthGraphicsView->setDate(m_showDate);
}

/**
 * @brief CMonthView::setHuangliInfo        设置黄历信息
 * @param huangliInfo
 */
void CMonthView::setHuangliInfo(const QMap<QDate, CaHuangLiDayInfo> &huangliInfo)
{
    m_monthGraphicsView->setLunarInfo(huangliInfo);
}

/**
 * @brief CMonthView::setFestival       设置班休信息
 * @param festivalInfo
 */
void CMonthView::setFestival(const QMap<QDate, int> &festivalInfo)
{
    m_monthGraphicsView->setFestival(festivalInfo);
}

/**
 * @brief CMonthView::setScheduleInfo       设置显示日程
 * @param scheduleInfo
 */
void CMonthView::setScheduleInfo(const QMap<QDate, QVector<ScheduleDataInfo> > &scheduleInfo)
{
    m_monthGraphicsView->setScheduleInfo(scheduleInfo);
}

/**
 * @brief CMonthView::setSearchScheduleInfo     设置搜索日程
 * @param searchScheduleInfo
 */
void CMonthView::setSearchScheduleInfo(const QVector<ScheduleDataInfo> &searchScheduleInfo)
{
    m_monthGraphicsView->setSearchScheduleInfo(searchScheduleInfo);
}

/**
 * @brief CMonthView::setCurrentDate        设置当前时间
 * @param currentDate
 */
void CMonthView::setCurrentDate(const QDate &currentDate)
{
    m_weekIndicator->setCurrentDate(currentDate);
}

void CMonthView::setRemindWidgetTimeFormat(QString timeformat)
{
    m_remindWidget->setTimeFormat(timeformat);
}

void CMonthView::setLunarVisible(bool visible)
{
    m_monthGraphicsView->setLunarVisible(visible);
}

ScheduleDataInfo CMonthView::getScheduleInfo(const QDate &beginDate, const QDate &endDate)
{
    ScheduleDataInfo info;
    if (beginDate.daysTo(endDate) > 0) {
        info.setBeginDateTime(QDateTime(beginDate, QTime(0, 0, 0)));
        info.setEndDateTime(QDateTime(endDate, QTime(23, 59, 59)));
    } else {
        info.setBeginDateTime(QDateTime(endDate, QTime(0, 0, 0)));
        info.setEndDateTime(QDateTime(beginDate, QTime(23, 59, 00)));
    }
    info.setTitleName(tr("New Event"));
    info.setAllDay(true);
    info.setRemindData(RemindData(1, QTime(9, 0)));
    return info;
}
