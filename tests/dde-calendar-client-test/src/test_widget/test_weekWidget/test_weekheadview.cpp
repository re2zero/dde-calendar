/*
* Copyright (C) 2019 ~ 2021 Uniontech Software Technology Co.,Ltd.
*
* Author:     zhengxiaokang <zhengxiaokang@uniontech.com>
*
* Maintainer: zhengxiaokang <zhengxiaokang@uniontech.com>
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
#include "test_weekheadview.h"
#include "../third-party_stub/stub.h"

test_weekheadview::test_weekheadview()
{
    mWeekHeadView = new CWeekHeadView();
}

test_weekheadview::~test_weekheadview()
{
    delete mWeekHeadView;
    mWeekHeadView = nullptr;
}

QMap<QDate, CaHuangLiDayInfo> getHuangliDayInfo()
{
    QMap<QDate, CaHuangLiDayInfo> huanglidayinfo {};
    CaHuangLiDayInfo huangliinfo1 {};
    huangliinfo1.mGanZhiYear = "辛丑牛年"; //年干支
    huangliinfo1.mGanZhiMonth = "庚寅月"; //月干支
    huangliinfo1.mGanZhiDay = "辛卯日"; //日干支
    huangliinfo1.mLunarMonthName = "正月"; //农历月名称
    huangliinfo1.mLunarDayName = "初一"; //农历日名称
    huangliinfo1.mLunarLeapMonth = 4; //闰月
    huangliinfo1.mZodiac = "牛"; //生肖
    huangliinfo1.mTerm = ""; //农历节气
    huangliinfo1.mSolarFestival = "国庆节"; //阳历节日
    huangliinfo1.mLunarFestival = "除夕"; //农历节日
    huangliinfo1.mSuit = "嫁娶"; //黄历宜
    huangliinfo1.mAvoid = "入土"; //黄历忌

    huanglidayinfo.insert(QDate::currentDate(), huangliinfo1);
    return huanglidayinfo;
}

QVector<QDate> getDayList()
{
    QVector<QDate> dateList {};
    QDate currentDate = QDate::currentDate();
    dateList.append(currentDate);
    dateList.append(currentDate.addDays(1));
    dateList.append(currentDate.addDays(2));
    dateList.append(currentDate.addDays(3));
    dateList.append(currentDate.addDays(4));
    dateList.append(currentDate.addDays(5));
    dateList.append(currentDate.addDays(6));

    return dateList;
}
//void CWeekHeadView::setTheMe(int type)
TEST_F(test_weekheadview, setTheMe)
{
    mWeekHeadView->setTheMe(1);
    mWeekHeadView->setTheMe(2);
}

//void CWeekHeadView::setWeekDay(QVector<QDate> vDays, const QDate &selectDate)
TEST_F(test_weekheadview, setWeekDay)
{
    QVector<QDate> dateList {};
    dateList.append(getDayList().first());
    mWeekHeadView->setWeekDay(dateList, QDate::currentDate());
    mWeekHeadView->setWeekDay(getDayList(), QDate::currentDate());
}

//void CWeekHeadView::setLunarVisible(bool visible)
TEST_F(test_weekheadview, setLunarvisible)
{
    mWeekHeadView->setLunarVisible(false);
    mWeekHeadView->setLunarVisible(true);
}

//const QString CWeekHeadView::getCellDayNum(int pos)
TEST_F(test_weekheadview, getCellDayNum)
{
    mWeekHeadView->setWeekDay(getDayList(), QDate::currentDate());
    mWeekHeadView->getCellDayNum(4);
}

//const QDate CWeekHeadView::getCellDate(int pos)
TEST_F(test_weekheadview, getCellDate)
{
    mWeekHeadView->setWeekDay(getDayList(), QDate::currentDate());
    mWeekHeadView->getCellDate(4);
}

//const QString CWeekHeadView::getLunar(int pos)
TEST_F(test_weekheadview, getLunar)
{
    mWeekHeadView->setWeekDay(getDayList(), QDate::currentDate());
    mWeekHeadView->setHunagliInfo(getHuangliDayInfo());
    mWeekHeadView->getLunar(1);
}
