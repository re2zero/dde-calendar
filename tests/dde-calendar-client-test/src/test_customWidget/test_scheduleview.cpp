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
#include "test_scheduleview.h"

test_scheduleview::test_scheduleview()
{
    mScheduleView = new CScheduleView();
}

test_scheduleview::~test_scheduleview()
{
    delete mScheduleView;
    mScheduleView = nullptr;
}

QVector<ScheduleDataInfo> getScheduleData()
{
    ScheduleDataInfo schedule1, schedule2;

    QDateTime currentDateTime = QDateTime::currentDateTime();
    schedule1.setID(1);
    schedule1.setBeginDateTime(currentDateTime);
    schedule1.setEndDateTime(currentDateTime.addDays(1));
    schedule1.setTitleName("scheduleOne");
    schedule1.setAllDay(true);
    schedule1.setType(1);
    schedule1.setRecurID(0);

    schedule2.setID(2);
    schedule2.setBeginDateTime(currentDateTime.addDays(1));
    schedule2.setEndDateTime(currentDateTime.addDays(1).addSecs(60 * 60));
    schedule2.setTitleName("scheduleTwo");
    schedule2.setAllDay(false);
    schedule2.setType(2);
    schedule2.setRecurID(0);

    QVector<ScheduleDataInfo> scheduleList{};
    scheduleList.append(schedule1);
    scheduleList.append(schedule2);
    return scheduleList;
}

//void CScheduleView::setviewMagin(int left, int top, int right, int bttom)
TEST_F(test_scheduleview, setviewMagin)
{
    mScheduleView->setviewMagin(8, 8, 4, 4);
}

//void CScheduleView::setRange(int w, int h, QDate begin, QDate end)
TEST_F(test_scheduleview, setRange)
{
    mScheduleView->setRange(4, 4, QDate::currentDate(), QDate::currentDate().addDays(1));
}

//void CScheduleView::setRange(QDate begin, QDate end)
TEST_F(test_scheduleview, setrange)
{
    mScheduleView->setRange(QDate::currentDate(), QDate::currentDate().addDays(1));
}

//void CScheduleView::setTheMe(int type)
TEST_F(test_scheduleview, setTheMe)
{
    mScheduleView->setTheMe(1);
    mScheduleView->setTheMe(2);
}

//void CScheduleView::setLunarVisible(bool state)
TEST_F(test_scheduleview, setLunarVisible)
{
    mScheduleView->setLunarVisible(true);
}

//void CScheduleView::setTime(QTime time)
TEST_F(test_scheduleview, setTime)
{
    mScheduleView->setTime(QTime::currentTime());
}

//void CScheduleView::setSelectSchedule(const ScheduleDataInfo &scheduleInfo)
TEST_F(test_scheduleview, setSelectSchedule)
{
    ScheduleDataInfo scheduleinfo = getScheduleData().first();
    mScheduleView->setSelectSchedule(scheduleinfo);
    scheduleinfo = getScheduleData().at(1);
    mScheduleView->setSelectSchedule(scheduleinfo);
}

//void CScheduleView::updateHigh()
TEST_F(test_scheduleview, updateHigh)
{
    mScheduleView->updateHigh();
}

//bool CScheduleView::IsDragging()
TEST_F(test_scheduleview, IsDragging)
{
    mScheduleView->IsDragging();
}

//void CScheduleView::setCurrentDate(const QDateTime &currentDate)
TEST_F(test_scheduleview, setCurrentDate)
{
    mScheduleView->setCurrentDate(QDateTime::currentDateTime());
}

//void CScheduleView::setShowScheduleInfo(const QMap<QDate, QVector<ScheduleDataInfo> > &scheduleInfo)
TEST_F(test_scheduleview, setShowScheduleInfo)
{
    QMap<QDate, QVector<ScheduleDataInfo> > dateInfoMap;
    dateInfoMap.insert(QDate::currentDate(), getScheduleData());
    mScheduleView->setShowScheduleInfo(dateInfoMap);
}

//void CScheduleView::setDate(QDate date)
TEST_F(test_scheduleview, setDate)
{
    mScheduleView->setDate(QDate::currentDate());
}

//void CScheduleView::slotupdateSchedule()
TEST_F(test_scheduleview, slotupdateSchedule)
{
    mScheduleView->slotupdateSchedule();
}

//void CScheduleView::slotPosHours(QVector<int> vPos, QVector<int> vHours, int cuttrnttimetype)
TEST_F(test_scheduleview, slotPosHours)
{
    QVector<int> pos;
    pos.append(1);
    pos.append(2);
    pos.append(3);
    pos.append(4);

    QVector<int> hours;
    hours.append(4);
    hours.append(8);
    hours.append(16);
    hours.append(20);

    mScheduleView->slotPosHours(pos, hours, 1);
}

//void CScheduleView::slotCurrentScheduleDate(QDate date)
TEST_F(test_scheduleview, slotCurrentScheduleDate)
{
    mScheduleView->slotCurrentScheduleDate(QDate::currentDate());
}

//void CScheduleView::slotScheduleShow(const bool isShow, const ScheduleDataInfo &out)
TEST_F(test_scheduleview, slotScheduleShow)
{
    mScheduleView->slotScheduleShow(false, getScheduleData().first());
    mScheduleView->slotScheduleShow(true, getScheduleData().at(1));
}

//void CScheduleView::slotUpdatePaint(const int topM)
TEST_F(test_scheduleview, slotUpdatePaint)
{
    const int top = 4;
    mScheduleView->slotUpdatePaint(top);
}

//void CScheduleView::slotUpdateScene()
TEST_F(test_scheduleview, slotUpdateScene)
{
    mScheduleView->slotUpdateScene();
}

//void CScheduleView::updateSchedule()
TEST_F(test_scheduleview, updateSchedule)
{
    mScheduleView->updateSchedule();
}

//void CScheduleView::updateAllday()
TEST_F(test_scheduleview, updateAllday)
{
    mScheduleView->updateAllday();
}

//int CScheduleView::checkDay(int weekday)
TEST_F(test_scheduleview, checkDay)
{
    mScheduleView->checkDay(-1);
    mScheduleView->checkDay(8);
}

//int CScheduleView::scheduleViewHegith()
TEST_F(test_scheduleview, scheduleViewHegith)
{
    mScheduleView->scheduleViewHegith();
}

