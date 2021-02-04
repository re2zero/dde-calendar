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
#include "test_scheduledlg.h"

test_scheduledlg::test_scheduledlg()
{
    mScheduleDlg = new CScheduleDlg(1);
}

test_scheduledlg::~test_scheduledlg()
{
    delete mScheduleDlg;
    mScheduleDlg = nullptr;
}

QVector<ScheduleDataInfo> getScheduleDlgData()
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
    schedule2.setAllDay(true);
    schedule2.setType(2);
    schedule2.setRecurID(0);

    QVector<ScheduleDataInfo> scheduleList{};
    scheduleList.append(schedule1);
    scheduleList.append(schedule2);
    return scheduleList;
}
//void CScheduleDlg::setData(const ScheduleDataInfo &info)
TEST_F(test_scheduledlg, setData)
{
    mScheduleDlg->setData(getScheduleDlgData().first());
}

//void CScheduleDlg::setDate(const QDateTime &date)
TEST_F(test_scheduledlg, setDate)
{
    QDateTime datetime1 = QDateTime(QDate::currentDate(), QTime(23, 54, 55));
    mScheduleDlg->setDate(datetime1);

    QDateTime datetime11 = QDateTime(QDate::currentDate(), QTime(13, 54, 55));
    mScheduleDlg->setDate(datetime1);

    QDateTime datetime2 = QDateTime(QDate::currentDate(), QTime(13, 40, 55));
    mScheduleDlg->setDate(datetime2);
}

//ScheduleDataInfo CScheduleDlg::getScheduleData()
TEST_F(test_scheduledlg, getScheduleData)
{
    mScheduleDlg->getScheduleData();
}

//void CScheduleDlg::setAllDay(bool flag)
TEST_F(test_scheduledlg, setAllDay)
{
    mScheduleDlg->setAllDay(true);
}

//bool CScheduleDlg::clickOkBtn()
TEST_F(test_scheduledlg, clickOkBtn)
{
    mScheduleDlg->clickOkBtn();
}

//void CScheduleDlg::slotBtClick(int buttonIndex, QString buttonName)
TEST_F(test_scheduledlg, slotBtClick)
{
    mScheduleDlg->slotBtClick(2, "ok");
}

//void CScheduleDlg::slotTextChange()
TEST_F(test_scheduledlg, slotTextChange)
{
    mScheduleDlg->slotTextChange();
}

//void CScheduleDlg::slotendrepeatTextchange()
TEST_F(test_scheduledlg, slotendrepeaTextchange)
{
    mScheduleDlg->slotendrepeatTextchange();
}

//void CScheduleDlg::slotBDateEidtInfo(const QDate &date)
TEST_F(test_scheduledlg, slotBDateEidtInfo)
{
    mScheduleDlg->slotBDateEidtInfo(QDate::currentDate());
}

//void CScheduleDlg::slotallDayStateChanged(int state)
TEST_F(test_scheduledlg, slotallDayStateCheanged)
{
    mScheduleDlg->slotallDayStateChanged(1);
    mScheduleDlg->slotallDayStateChanged(0);
}

//void CScheduleDlg::slotbRpeatactivated(int index)
TEST_F(test_scheduledlg, slotbRpearactivated)
{
    mScheduleDlg->slotbRpeatactivated(0);
    mScheduleDlg->slotbRpeatactivated(2);
}

//void CScheduleDlg::sloteRpeatactivated(int index)
TEST_F(test_scheduledlg, sloteRpearactivated)
{
    mScheduleDlg->sloteRpeatactivated(0);
    mScheduleDlg->sloteRpeatactivated(1);
    mScheduleDlg->sloteRpeatactivated(2);
}

//void CScheduleDlg::setTheMe(const int type)
TEST_F(test_scheduledlg, setTheMe)
{
    mScheduleDlg->setTheMe(1);
    mScheduleDlg->setTheMe(2);
}
