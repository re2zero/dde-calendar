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
#include "test_daymonthview.h"

test_daymonthview::test_daymonthview()
{
    mDayMonthView = new CDayMonthView();
}

test_daymonthview::~test_daymonthview()
{
    delete mDayMonthView;
    mDayMonthView = nullptr;
}

QVector<QDate> dayMonthviewGetDayList()
{
    QVector<QDate> dateList {};
    QDate currentDate = QDate::currentDate();
    for (int i = 0; i < 42; i++) {
        dateList.append(currentDate.addDays(i));
    }

    return dateList;
}

QMap<QDate, CaHuangLiDayInfo> dayMonthViewGetHuangliDayInfo()
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

QVector<bool> dayMonthViewGetLineFlag()
{
    QVector<bool> flags {};
    for (int i = 0; i < 42; i++) {
        flags.append(true);
    }
    return flags;
}

//void CDayMonthView::setShowDate(const QVector<QDate> &showDate, const QDate &selectDate, const QDate &currentDate)
TEST_F(test_daymonthview, setShowDate)
{
    mDayMonthView->setShowDate(dayMonthviewGetDayList(), QDate::currentDate(), QDate::currentDate());
}

//void CDayMonthView::setLunarVisible(bool visible)
TEST_F(test_daymonthview, setLunarVisible)
{
    mDayMonthView->setLunarVisible(true);
}

//void CDayMonthView::setTheMe(int type)
TEST_F(test_daymonthview, setTheMe)
{
    mDayMonthView->setTheMe(1);
    mDayMonthView->setTheMe(2);
}

//void CDayMonthView::setSearchFlag(bool flag)
TEST_F(test_daymonthview, setSearchFlag)
{
    mDayMonthView->setSearchFlag(true);
}

//void CDayMonthView::setHuangliInfo(const CaHuangLiDayInfo &huangliInfo)
TEST_F(test_daymonthview, setHuangliInfo)
{
    mDayMonthView->setHuangliInfo(dayMonthViewGetHuangliDayInfo().value(QDate::currentDate()));
}

//void CDayMonthView::setHasScheduleFlag(const QVector<bool> &hasScheduleFlag)
TEST_F(test_daymonthview, setHasScheduleFlag)
{
    mDayMonthView->setHasScheduleFlag(dayMonthViewGetLineFlag());
}

//void CDayMonthView::updateDateShow()
TEST_F(test_daymonthview, updateDateShow)
{
    mDayMonthView->setShowDate(dayMonthviewGetDayList(), QDate::currentDate(), QDate::currentDate());
    mDayMonthView->updateDateShow();
    mDayMonthView->setShowDate(dayMonthviewGetDayList(), QDate::currentDate().addDays(1), QDate::currentDate().addDays(1));
    mDayMonthView->updateDateShow();
}

//void CDayMonthView::updateDateLunarDay()
TEST_F(test_daymonthview, updateDateLunarDay)
{
    mDayMonthView->updateDateLunarDay();
}

//const QString CDayMonthView::getCellDayNum(int pos)
TEST_F(test_daymonthview, getCellDayNum)
{
    mDayMonthView->setShowDate(dayMonthviewGetDayList(), QDate::currentDate(), QDate::currentDate());
    mDayMonthView->getCellDayNum(2);
}

//const QDate CDayMonthView::getCellDate(int pos)
TEST_F(test_daymonthview, getCellDate)
{
    mDayMonthView->setShowDate(dayMonthviewGetDayList(), QDate::currentDate(), QDate::currentDate());
    mDayMonthView->getCellDate(2);
}

//void CDayMonthView::cellClicked(QWidget *cell)
TEST_F(test_daymonthview, cellDlicked)
{
    QWidget *widget;
    mDayMonthView->cellClicked(widget);
}

//void CDayMonthView::setSelectedCell(int index)
TEST_F(test_daymonthview, setSelectedCell)
{
    mDayMonthView->setShowDate(dayMonthviewGetDayList(), QDate::currentDate().addDays(1), QDate::currentDate().addDays(1));
    mDayMonthView->setSelectedCell(4);
}

//void CDayMonthView::changeSelectDate(const QDate &date)
TEST_F(test_daymonthview, changeSelectDate)
{
    mDayMonthView->changeSelectDate(QDate::currentDate());
}

//void CDayMonthView::slotprev()
TEST_F(test_daymonthview, slotprev)
{
    mDayMonthView->setShowDate(dayMonthviewGetDayList(), QDate::currentDate().addDays(1), QDate::currentDate().addDays(1));
    mDayMonthView->slotprev();
}

//void CDayMonthView::slotnext()
TEST_F(test_daymonthview, slotnext)
{
    mDayMonthView->setShowDate(dayMonthviewGetDayList(), QDate::currentDate().addDays(1), QDate::currentDate().addDays(1));
    mDayMonthView->slotnext();
}

//void CDayMonthView::slottoday()
TEST_F(test_daymonthview, slottoday)
{
    mDayMonthView->setShowDate(dayMonthviewGetDayList(), QDate::currentDate().addDays(1), QDate::currentDate().addDays(1));
    mDayMonthView->slottoday();
}
