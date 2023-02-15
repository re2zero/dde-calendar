/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     chenhaifeng  <chenhaifeng@uniontech.com>
*
* Maintainer: chenhaifeng  <chenhaifeng@uniontech.com>
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
#include "testscheduledata.h"

TestScheduleData::TestScheduleData()
{
}

QVector<ScheduleDataInfo> TestDataInfo::getScheduleItemDInfo()
{
    QVector<ScheduleDataInfo> scheduleDate {};
    ScheduleDataInfo schedule1, schedule2, schedule3, schedule4, schedule5, scheduleFes;
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
    schedule2.setAllDay(true);
    schedule2.setType(2);
    schedule2.setRecurID(0);

    schedule3.setID(3);
    schedule3.setBeginDateTime(currentDateTime.addDays(2));
    schedule3.setEndDateTime(currentDateTime.addDays(2).addSecs(60 * 60));
    schedule3.setTitleName("scheduleThree");
    schedule3.setAllDay(false);
    schedule3.setType(3);
    schedule3.setRecurID(0);

    schedule4.setID(4);
    schedule4.setBeginDateTime(currentDateTime.addDays(3));
    schedule4.setEndDateTime(currentDateTime.addDays(3).addSecs(60 * 60));
    schedule4.setTitleName("scheduleFour");
    schedule4.setAllDay(false);
    schedule4.setType(1);
    schedule4.setRecurID(0);

    schedule5.setID(5);
    schedule5.setBeginDateTime(currentDateTime.addDays(4));
    schedule5.setEndDateTime(currentDateTime.addDays(4).addSecs(60 * 60));
    schedule5.setTitleName("scheduleFive");
    schedule5.setAllDay(false);
    schedule5.setType(2);
    schedule5.setRecurID(0);

    scheduleFes.setID(6);
    scheduleFes.setBeginDateTime(currentDateTime.addDays(5));
    scheduleFes.setEndDateTime(currentDateTime.addDays(5).addSecs(60 * 60));
    scheduleFes.setTitleName("scheduleFestival");
    scheduleFes.setAllDay(true);
    scheduleFes.setType(4);
    scheduleFes.setRecurID(0);

    scheduleDate.append(schedule1);
    scheduleDate.append(schedule2);
    scheduleDate.append(schedule3);
    scheduleDate.append(schedule4);
    scheduleDate.append(schedule5);
    scheduleDate.append(scheduleFes);
    return scheduleDate;
}
