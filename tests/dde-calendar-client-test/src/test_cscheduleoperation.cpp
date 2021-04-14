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
#include "test_cscheduleoperation.h"

#include "cscheduledbus.h"
#include "dialog/dcalendarddialog.h"
#include "schedulectrldlg.h"
#include "dialog_stub.h"
#include "cscheduledbusstub.h"

test_cscheduleoperation::test_cscheduleoperation()
{
}

test_cscheduleoperation::~test_cscheduleoperation()
{
}



namespace ScheduleTestBtnNum {
static int button_num = 0;
}
int clickButton_stub(void *obj)
{
    Q_UNUSED(obj)
    return ScheduleTestBtnNum::button_num;
}

void test_cscheduleoperation::SetUp()
{
    cscheduleDbusStub(stub);
}

void test_cscheduleoperation::TearDown()
{
}

TEST_F(test_cscheduleoperation, createSchedule)
{
    ScheduleDataInfo info;
    operation.createSchedule(info);
}

TEST_F(test_cscheduleoperation, changeSchedule)
{
    calendarDDialogExecStub(stub);
    stub.set(ADDR(CScheduleCtrlDlg, clickButton), clickButton_stub);
    ScheduleDataInfo info;
    QDate current = QDate::currentDate();
    info.setBeginDateTime(QDateTime(current, QTime(0, 0, 0)));
    info.setEndDateTime(QDateTime(current, QTime(23, 59, 59)));
    info.setTitleName(tr("New Event"));
    info.setAllDay(true);
    info.setRemindData(RemindData(1, QTime(9, 0)));
    info.setID(0);
    info.setRecurID(0);

    ScheduleDataInfo newinfo = info;
    newinfo.setTitleName(tr("test"));

    newinfo.setAllDay(false);
    info.getRepetitionRule().setRuleId(RepetitionRule::RRule_NONE);
    ScheduleTestBtnNum::button_num = 0;
    operation.changeSchedule(newinfo, info);
    ScheduleTestBtnNum::button_num = 1;
    operation.changeSchedule(newinfo, info);

    newinfo.setAllDay(true);
    operation.changeSchedule(newinfo, info);
    ScheduleTestBtnNum::button_num = 0;
    newinfo.getRepetitionRule().setRuleId(RepetitionRule::RRule_EVEDAY);
    operation.changeSchedule(newinfo, info);
    ScheduleTestBtnNum::button_num = 1;
    operation.changeSchedule(newinfo, info);
    info.getRepetitionRule().setRuleId(RepetitionRule::RRule_EVEDAY);
    operation.changeSchedule(newinfo, info);
    ScheduleTestBtnNum::button_num = 0;
    operation.changeSchedule(newinfo, info);
    ScheduleTestBtnNum::button_num = 2;
    operation.changeSchedule(newinfo, info);

    newinfo.setRecurID(2);
    ScheduleTestBtnNum::button_num = 0;
    operation.changeSchedule(newinfo, info);
    ScheduleTestBtnNum::button_num = 1;
    operation.changeSchedule(newinfo, info);
    ScheduleTestBtnNum::button_num = 2;
    operation.changeSchedule(newinfo, info);
}

TEST_F(test_cscheduleoperation, deleteSchedule)
{
    calendarDDialogExecStub(stub);
    stub.set(ADDR(CScheduleCtrlDlg, clickButton), clickButton_stub);
    ScheduleDataInfo info;
    QDate current = QDate::currentDate();
    info.setBeginDateTime(QDateTime(current, QTime(0, 0, 0)));
    info.setEndDateTime(QDateTime(current, QTime(23, 59, 59)));
    info.setTitleName(tr("New Event"));
    info.setAllDay(true);
    info.setRemindData(RemindData(1, QTime(9, 0)));
    info.setID(0);
    info.setRecurID(0);
    ScheduleTestBtnNum::button_num = 0;
    operation.deleteSchedule(info);
    ScheduleTestBtnNum::button_num = 1;
    operation.deleteSchedule(info);

    //删除重复日程
    info.getRepetitionRule().setRuleId(RepetitionRule::RRule_EVEDAY);
    ScheduleTestBtnNum::button_num = 0;
    operation.deleteSchedule(info);
    ScheduleTestBtnNum::button_num = 1;
    operation.deleteSchedule(info);
    ScheduleTestBtnNum::button_num = 2;
    operation.deleteSchedule(info);

    info.setRecurID(2);
    ScheduleTestBtnNum::button_num = 0;
    operation.deleteSchedule(info);
    ScheduleTestBtnNum::button_num = 1;
    operation.deleteSchedule(info);
    ScheduleTestBtnNum::button_num = 2;
    operation.deleteSchedule(info);
}

TEST_F(test_cscheduleoperation, queryScheduleStr)
{
    QDateTime currenttime = QDateTime::currentDateTime();
    operation.queryScheduleStr("", currenttime, currenttime);
}

TEST_F(test_cscheduleoperation, deleteOnlyInfo)
{
    ScheduleDataInfo info;
    operation.deleteOnlyInfo(info);
}
