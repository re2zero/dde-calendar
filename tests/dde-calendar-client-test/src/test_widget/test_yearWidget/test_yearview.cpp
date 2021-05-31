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
#include "test_yearview.h"

test_yearview::test_yearview()
{
    cYearView = new CYearView();
    cMonthBrefWidget = new MonthBrefWidget();
}

test_yearview::~test_yearview()
{
    delete cYearView;
    cYearView = nullptr;
    delete cMonthBrefWidget;
    cMonthBrefWidget = nullptr;
}

QVector<QDate> getListDate()
{
    QVector<QDate> listDate {};
    QDate date = QDate::currentDate();
    for (int i = 0; i < 42; i++) {
        listDate.append(date);
        date = date.addDays(1);
    }
    return listDate;
}

//void CYearView::slotDoubleClickDate(const QDate &date)
TEST_F(test_yearview, slotDoubltClickDate)
{
    cYearView->slotDoubleClickDate(QDate(2021, 1, 6));
}

//void CYearView::slotPressClickDate(const QDate &date)
TEST_F(test_yearview, slotPressClickDate)
{
    cYearView->slotPressClickDate(QDate(2021, 1, 6));
}

//void CYearView::setTheMe(int type)
TEST_F(test_yearview, setTheMe)
{
    cYearView->setTheMe(1);
    cYearView->setTheMe(2);
}

//void CYearView::setShowDate(const QDate &showMonth, const QVector<QDate> &showDate)
TEST_F(test_yearview, setShowDate)
{
    cYearView->setShowDate(getListDate().first(), getListDate());
}

//void CYearView::setHasScheduleFlag(const QVector<bool> &hasScheduleFlag)
TEST_F(test_yearview, setHasScheduleFlag)
{
    QVector<bool> listLintFlag {};
    listLintFlag.append(false);
    listLintFlag.append(true);

    cYearView->setHasScheduleFlag(listLintFlag);
}

//void CYearView::setHasSearchScheduleFlag(const QVector<bool> &hasSearchScheduleFlag)
TEST_F(test_yearview, setHasSearchScheduleFlag)
{
    QVector<bool> listLintFlag {};
    listLintFlag.append(false);
    listLintFlag.append(true);

    cYearView->setHasSearchScheduleFlag(listLintFlag);
}

//bool CYearView::getStartAndStopDate(QDate &startDate, QDate &stopDate)
TEST_F(test_yearview, getStartAndStopDate)
{
    QDate startDate(QDate(2021, 1, 7));
    QDate stopDate(QDate(2021, 1, 8));

    cYearView->setShowDate(getListDate().first(), getListDate());
    bool result = cYearView->getStartAndStopDate(startDate, stopDate);
    EXPECT_TRUE(result);
}

TEST_F(test_yearview, paintEvent)
{
    cYearView->setFixedSize(600, 600);
    QPixmap pixmap(cYearView->size());
    cYearView->render(&pixmap);
}
