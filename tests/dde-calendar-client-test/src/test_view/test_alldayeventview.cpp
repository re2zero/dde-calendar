/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     hejinghai <hejinghai@uniontech.com>
*
* Maintainer: hejinghai <hejinghai@uniontech.com>
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
#include "test_alldayeventview.h"
#include "../dialog_stub.h"

#include <QTest>

test_alldayeventview::test_alldayeventview()
{
    cAllDayEventWeekView = new CAllDayEventWeekView();
}

test_alldayeventview::~test_alldayeventview()
{
    delete cAllDayEventWeekView;
    cAllDayEventWeekView = nullptr;
}

//void CAllDayEventWeekView::setTheMe(int type)
TEST_F(test_alldayeventview, setTheMe)
{
    int type = 1;
    cAllDayEventWeekView->setTheMe();
    cAllDayEventWeekView->setTheMe(type);
}

//bool CAllDayEventWeekView::MeetCreationConditions(const QDateTime &date)
TEST_F(test_alldayeventview, MeetCreationConditions)
{
    QString begin = "2020-12-01 12:24:36";
    QDateTime date = QDateTime::fromString(begin, "yyyy-MM-dd hh:mm:ss");
    cAllDayEventWeekView->MeetCreationConditions(date);
}

//bool CAllDayEventWeekView::IsEqualtime(const QDateTime &timeFirst, const QDateTime &timeSecond)
TEST_F(test_alldayeventview, IsEqualtime)
{
    QString begin = "2020-12-01 12:24:36";
    QDateTime begindate = QDateTime::fromString(begin, "yyyy-MM-dd hh:mm:ss");
    QString end = "2020-12-21 12:24:36";
    QDateTime enddate = QDateTime::fromString(end, "yyyy-MM-dd hh:mm:ss");

    bool getBool = cAllDayEventWeekView->IsEqualtime(begindate, enddate);
    assert(false == getBool);

    enddate = QDateTime::fromString(begin, "yyyy-MM-dd hh:mm:ss");
    getBool = cAllDayEventWeekView->IsEqualtime(begindate, enddate);
    assert(true == getBool);
}

//void CAllDayEventWeekView::setRange(int w, int h, QDate begindate, QDate enddate, int rightmagin)
TEST_F(test_alldayeventview, setRange)
{
    int w = 3;
    int h = 2;
    QDate begindate(2020, 12, 01);
    QDate enddate(2020, 12, 21);
    int rightmagin = 2;
    cAllDayEventWeekView->setRange(w, h, begindate, enddate, rightmagin);
}

QVector<ScheduleDataInfo> getTestScheduleDataInfo()
{
    ScheduleDataInfo info1;
    info1.setID(1);
    info1.setType(2);
    info1.setAllDay(true);
    info1.setRecurID(2);
    info1.setTitleName("测试1");
    QString strDate = "2020-12-02 12:24:36";
    QDateTime ignoreDate = QDateTime::fromString(strDate, "yyyy-MM-dd hh:mm:ss");
    QVector<QDateTime> ignoreDateList;
    ignoreDateList.append(ignoreDate);
    info1.setIgnoreTime(ignoreDateList);
    QString begin = "2020-12-01 12:24:36";
    QDateTime begindate = QDateTime::fromString(begin, "yyyy-MM-dd hh:mm:ss");
    QString end = "2020-12-03 12:24:36";
    QDateTime enddate = QDateTime::fromString(end, "yyyy-MM-dd hh:mm:ss");
    info1.setBeginDateTime(begindate);
    info1.setEndDateTime(enddate);

    ScheduleDataInfo info2;
    info2.setID(1);
    info2.setType(2);
    info2.setAllDay(true);
    info2.setRecurID(2);
    info2.setTitleName("测试2");
    QString strDate2 = "2020-12-12 12:24:36";
    QDateTime ignoreDate2 = QDateTime::fromString(strDate2, "yyyy-MM-dd hh:mm:ss");
    QVector<QDateTime> ignoreDateList2;
    ignoreDateList2.append(ignoreDate2);
    info2.setIgnoreTime(ignoreDateList2);
    QString begin2 = "2020-12-11 12:24:36";
    QDateTime begindate2 = QDateTime::fromString(begin2, "yyyy-MM-dd hh:mm:ss");
    QString end2 = "2020-12-13 12:24:36";
    QDateTime enddate2 = QDateTime::fromString(end2, "yyyy-MM-dd hh:mm:ss");
    info2.setBeginDateTime(begindate2);
    info2.setEndDateTime(enddate2);

    ScheduleDataInfo info3;
    info3.setID(1);
    info3.setType(2);
    info3.setAllDay(true);
    info3.setRecurID(2);
    info3.setTitleName("测试3");
    QString strDate3 = "2020-12-22 12:24:36";
    QDateTime ignoreDate3 = QDateTime::fromString(strDate3, "yyyy-MM-dd hh:mm:ss");
    QVector<QDateTime> ignoreDateList3;
    ignoreDateList3.append(ignoreDate3);
    info3.setIgnoreTime(ignoreDateList3);
    QString begin3 = "2020-12-21 12:24:36";
    QDateTime begindate3 = QDateTime::fromString(begin3, "yyyy-MM-dd hh:mm:ss");
    QString end3 = "2020-12-23 12:24:36";
    QDateTime enddate3 = QDateTime::fromString(end3, "yyyy-MM-dd hh:mm:ss");
    info3.setBeginDateTime(begindate3);
    info3.setEndDateTime(enddate3);

    QVector<ScheduleDataInfo> infoList;
    infoList.append(info1);
    infoList.append(info2);
    infoList.append(info3);

    return infoList;
}

//void CAllDayEventWeekView::setDayData(const QVector<QVector<ScheduleDataInfo>> &vlistData)
TEST_F(test_alldayeventview, setDayData)
{
    QVector<QVector<ScheduleDataInfo>> vlistData;

    QVector<ScheduleDataInfo> infoList1;
    infoList1.append(getTestScheduleDataInfo().at(0));
    infoList1.append(getTestScheduleDataInfo().at(1));

    QVector<ScheduleDataInfo> infoList2;
    infoList2.append(getTestScheduleDataInfo().at(2));

    vlistData.append(infoList1);
    vlistData.append(infoList2);

    cAllDayEventWeekView->setDayData(vlistData);
}


//void CAllDayEventWeekView::setInfo(const QVector<ScheduleDataInfo> &info)
TEST_F(test_alldayeventview, setInfo)
{
    cAllDayEventWeekView->setInfo(getTestScheduleDataInfo());
}

//void CAllDayEventWeekView::slotDoubleEvent()
TEST_F(test_alldayeventview, slotDoubleEvent)
{
    cAllDayEventWeekView->slotDoubleEvent();
}

//void CAllDayEventWeekView::createItemWidget(int index, bool average)
TEST_F(test_alldayeventview, createItemWidget)
{
    QVector<QVector<ScheduleDataInfo>> vlistData;

    QVector<ScheduleDataInfo> infoList1;
    infoList1.append(getTestScheduleDataInfo().at(0));
    infoList1.append(getTestScheduleDataInfo().at(1));

    QVector<ScheduleDataInfo> infoList2;
    infoList2.append(getTestScheduleDataInfo().at(2));

    vlistData.append(infoList1);
    vlistData.append(infoList2);

    cAllDayEventWeekView->setDayData(vlistData);

    int w = 3;
    int h = 2;
    QDate begindate(2020, 12, 01);
    QDate enddate(2020, 12, 21);
    int rightmagin = 2;
    cAllDayEventWeekView->setRange(w, h, begindate, enddate, rightmagin);

    int index = 0;
    bool average = true;
    cAllDayEventWeekView->createItemWidget(index, average);
}

//void CAllDayEventWeekView::updateItemHeightByFontSize()
TEST_F(test_alldayeventview, updateItemHeightByFontSize)
{
    cAllDayEventWeekView->updateItemHeightByFontSize();
}


//void CAllDayEventWeekView::upDateInfoShow(const DragStatus &status, const ScheduleDataInfo &info)
TEST_F(test_alldayeventview, upDateInfoShow)
{
    cAllDayEventWeekView->setInfo(getTestScheduleDataInfo());
    cAllDayEventWeekView->upDateInfoShow(DragInfoGraphicsView::DragStatus::ChangeEnd, getTestScheduleDataInfo().at(1));
}

//
TEST_F(test_alldayeventview, getPixmap)
{
    cAllDayEventWeekView->setFixedSize(500, 800);
    QPixmap pixmap(cAllDayEventWeekView->size());
    pixmap = cAllDayEventWeekView->grab();
}

//
TEST_F(test_alldayeventview, eventTest)
{
    QTest::keyEvent(QTest::Press, cAllDayEventWeekView, Qt::Key::Key_Enter);
    cAllDayEventWeekView->setFixedSize(500, 100);
    QTest::mouseDClick(cAllDayEventWeekView, Qt::LeftButton, Qt::NoModifier, QPoint(200, 50));
}

//slotCreate
TEST_F(test_alldayeventview, slotCreate)
{
    Stub stub;
    calendarDDialogExecStub(stub);
    cAllDayEventWeekView->slotCreate(QDateTime::currentDateTime());
}

//getDragScheduleInfoBeginTime
TEST_F(test_alldayeventview, getDragScheduleInfoBeginTime)
{
    cAllDayEventWeekView->getDragScheduleInfoBeginTime(QDateTime::currentDateTime());
}
