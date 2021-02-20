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
#include "test_monthgraphiview.h"

test_monthgraphiview::test_monthgraphiview()
{
    cMonthGraphiview = new CMonthGraphiview();
}

test_monthgraphiview::~test_monthgraphiview()
{
    delete cMonthGraphiview;
}

//void CMonthGraphiview::setTheMe(int type)
TEST_F(test_monthgraphiview, setTheMe)
{
    int type = 1;
    cMonthGraphiview->setTheMe();
    cMonthGraphiview->setTheMe(type);
}

//void CMonthGraphiview::setDate(const QVector<QDate> &showDate)
TEST_F(test_monthgraphiview, setDate)
{
    QVector<QDate> showDate;
    QDate date1(2020, 10, 1);
    QDate date2(2020, 12, 1);
    for (int i = 0; i < 42; ++i) {
        if (i % 2 == 0) {
            showDate.append(date1);
        } else {
            showDate.append(date2);
        }
    }
    std::cout << showDate.size() << std::endl;
    cMonthGraphiview->setDate(showDate);
}

//void CMonthGraphiview::setLunarInfo(const QMap<QDate, CaHuangLiDayInfo> &lunarCache)
TEST_F(test_monthgraphiview, setLunarInfo)
{
    QMap<QDate, CaHuangLiDayInfo> lunarCache = QMap<QDate, CaHuangLiDayInfo> {};
    cMonthGraphiview->setLunarInfo(lunarCache);
}

//void CMonthGraphiview::setLunarVisible(bool visible)
TEST_F(test_monthgraphiview, setLunarVisible)
{
    bool visible = false;
    cMonthGraphiview->setLunarVisible(visible);
}

//void CMonthGraphiview::setScheduleInfo(const QMap<QDate, QVector<ScheduleDataInfo> > &info)
TEST_F(test_monthgraphiview, setScheduleInfo)
{
    QMap<QDate, QVector<ScheduleDataInfo> > info = QMap<QDate, QVector<ScheduleDataInfo> > {};
    cMonthGraphiview->setScheduleInfo(info);
}

//void CMonthGraphiview::setSelectSearchSchedule(const ScheduleDataInfo &scheduleInfo)
TEST_F(test_monthgraphiview, setSelectSearchSchedule)
{
    ScheduleDataInfo scheduleInfo = ScheduleDataInfo{};
    cMonthGraphiview->setSelectSearchSchedule(scheduleInfo);
}

//QPointF CMonthGraphiview::getItemPos(const QPoint &p, const QRectF &itemRect)
TEST_F(test_monthgraphiview, getItemPos)
{
    QPoint p(100, 100);
    QRectF itemRect;
    cMonthGraphiview->getItemPos(p, itemRect);
}

//QDateTime CMonthGraphiview::getPosDate(const QPoint &p)
TEST_F(test_monthgraphiview, getPosDate)
{
    QPoint p(100, 100);
    QDateTime datetime = cMonthGraphiview->getPosDate(p);
    qInfo() << datetime;
}

////void CMonthGraphiview::upDateInfoShow(const CMonthGraphiview::DragStatus &status, const ScheduleDataInfo &info)
//TEST_F(test_monthgraphiview, upDateInfoShow)
//{
//    ScheduleDataInfo info = ScheduleDataInfo{};
//    cMonthGraphiview->upDateInfoShow(CMonthGraphiview::DragStatus::IsCreate, info);
//}






















